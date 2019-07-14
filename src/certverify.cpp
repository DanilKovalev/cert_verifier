#include "Instance.h"
#include "OutputDirectory.h"
#include "http/HttpClient.h"
#include "pkcs/Pkcs12.h"
#include "pkcs/Pkcs7Signed.h"
#include "utils/StackOf.h"
#include "x509/X509Certificate.h"
#include "x509/X509Store.h"
#include "x509/extensions/AuthorityInformationAccess.h"
#include "x509/extensions/CrlDistributionPoints.h"

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>

#include <x509/X509StoreCtx.h>

X509StoreCtx g_trustedStoreCtx;

namespace po = boost::program_options;

static po::variables_map init_options(int argc, char** argv)
{
    po::options_description desc("Allowed options", 1024);
    desc.add_options()("help,h", "produce help message");
    desc.add_options()("verbose,v", "verbose log mode");
    desc.add_options()("certificate", po::value<std::string>(), "certificate to validate");
    desc.add_options()(
      "output-directory", po::value<std::string>()->default_value("/tmp/certverifier"), "output directory");

    po::variables_map vm;
    po::positional_options_description p;
    p.add("certificate", 1);
    po::command_line_parser cmd(argc, argv);
    cmd.options(desc);
    cmd.positional(p);
    try
    {
        po::store(cmd.run(), vm);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what();
        std::cerr << desc;
        exit(1);
    }

    if (vm.count("certificate") == 0)
    {
        std::cerr << "Certificate doesn't set" << std::endl;
        std::cerr << desc;
        exit(1);
    }

    return vm;
}

bool is_pem(const std::string& line)
{
    return line.find("-----BEGIN CERTIFICATE-----") != std::string::npos;
}

X509Certificate read_certificate(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        std::__throw_system_error(errno);

    std::string line;
    std::getline(file, line);
    file.seekg(0);
    if (is_pem(line))
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return X509Certificate::from_pem(content);
    }
    else
    {
        std::vector<uint8_t> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return X509Certificate::from_der(content);
    }
}

X509Certificate download_certificate(const std::string& url)
{
    std::cout << "Downloading from: " << url << std::endl;
    std::vector<uint8_t> data = HttpClient::request(url);

    if (boost::algorithm::ends_with(url, ".p7c"))
    {
        Pkcs7Signed pkcs7(Pkcs7::fromDer(data));
        if (pkcs7.getCertificates().size() == 0)
            throw std::runtime_error("No certificates found in pkcs7");
        else if (pkcs7.getCertificates().size() > 1)
            throw std::runtime_error("More than one certificates stored in pkcs7");

        X509Certificate cert = pkcs7.getCertificates().front();
        cert.cloneIfNotAcquire();
        return cert;
    }

    return X509Certificate::from_der(data);
}

AuthorityInformationAccess getuthorityInformationAccess(X509Certificate certificate)
{
    X509ExtensionsStack extensions = certificate.get_extensions();
    auto authInfo = extensions.findExtension<AuthorityInformationAccess>();

    if (!authInfo.has_value())
        throw std::runtime_error("No Authority Information Access extension found for certificate");

    return authInfo.value();
}

CrlDistributionPoints getCrlDistributionPoints(X509Certificate& certificate)
{
    X509ExtensionsStack extensions = certificate.get_extensions();
    auto crlDistPoints = extensions.findExtension<CrlDistributionPoints>();

    if (!crlDistPoints.has_value())
        throw std::runtime_error("No CRL Distribution Points extension found for certificate");

    return crlDistPoints.value();
}

std::vector<X509Crl> GetCrls(X509Certificate& certificate)
{
    std::vector<X509Crl> result;
    CrlDistributionPoints crlDistPoints = getCrlDistributionPoints(certificate);
    for (DistPoint& point : crlDistPoints.getDistPoints())
    {
        for (std::string& url : point.get_distribution_point_names())
        {
            std::vector<uint8_t> data = HttpClient::request(url);
            result.push_back(X509Crl::fromDer(data));
        }
    }

    return result;
}

X509Certificate getIssuer(const X509Certificate& childCert)
{
    std::cout << "Looking: " << childCert.getIssuerName().toString() << std::endl;

    X509Name issuerName = childCert.getIssuerName();
    std::optional<X509Certificate> issuerCert = g_trustedStoreCtx.findCertificateBySubject(issuerName);
    if (issuerCert.has_value())
    {
        std::cout << "Found in trusted store" << std::endl;
        return issuerCert.value();
    }

    AuthorityInformationAccess infoAccess = getuthorityInformationAccess(childCert);
    return download_certificate(infoAccess.ca_issuer());
}

void foo(X509_OBJECT* obj)
{
    X509Certificate cert(X509_OBJECT_get0_X509(obj), true);
    std::cout << "trusted: " << X509_trusted(cert.raw()) << std::endl;
}

int main(int argc, char** argv)
{
    auto& ssl_instance = Instance::get();
    (void)ssl_instance;

    po::variables_map vm = init_options(argc, argv);

    OutputDirectory outDir(vm["output-directory"].as<std::string>());
    X509Store trustedStore;
    trustedStore.setTrust(true);
    trustedStore.loadDefaultLocation();

    X509StoreCtx trustedStoreCtx;
    g_trustedStoreCtx.setStore(std::move(trustedStore));

    Pkcs12Content pkcs12Data = Pkcs12Content::createEmpty();
    X509Certificate curCert = read_certificate(vm["certificate"].as<std::string>());
    pkcs12Data.cert = curCert;
    while (!curCert.isSelfSigned())
    {
        for (const X509Crl& crl : GetCrls(curCert))
            outDir.saveCrl(crl);

        X509Certificate parentCert = getIssuer(curCert);
        pkcs12Data.ca.push(parentCert);
        curCert = parentCert;
        std::cout << "--------" << std::endl;
    }

    outDir.savePkcs12(Pkcs12::create(pkcs12Data, ""));
}
