#include "Instance.h"
#include "x509/X509Certificate.h"
#include "x509/extensions/AuthorityInformationAccess.h"
#include "utils/StackOf.h"
#include "http/HttpClient.h"

#include "Pkcs12.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

namespace po = boost::program_options;

static po::variables_map init_options(int argc, char** argv)
{
    po::options_description desc("Allowed options", 1024);
    desc.add_options()
            ("help,h", "produce help message")
            ("verbose,v", "verbose log mode")
            ;

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
    catch(std::exception &e){
        std::cerr<<e.what();
        std::cerr<<desc;
        exit(1);
    }

    if(vm.count("verbose")){
        std::cout<<"verbose";
    }

    if(vm.count("certificate") == 0)
    {
        std::cerr<<"Certificate doesn't set" << std::endl;
        std::cerr<<desc;
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
    if (is_pem( line) )
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return X509Certificate::from_pem(content);
    }
    else
    {
        std::vector<uint8_t > content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return  X509Certificate::from_der(content);
    }
}

std::unique_ptr<AuthorityInformationAccess> findAuthorityInformationAccess(X509Certificate certificate)
{
    StackOf<X509Extension> extensions = certificate.get_extensions();
    auto it = std::find_if(extensions.begin(), extensions.end(),
                           [](const X509Extension& ext) -> bool {
                               return ext.nid() == NID_info_access;
                           });

    if (it == extensions.end())
        return {};

    return std::make_unique<AuthorityInformationAccess>(*it);
}

int main(int argc, char** argv)
{
    po::variables_map vm = init_options(argc, argv);

    Pkcs12Data data;

    data.cert = read_certificate(vm["certificate"].as<std::string>());
    StackOf<X509Certificate> additionalCerts;

    StackOf<X509Certificate> additionalCertificates;
    X509Certificate curCert = data.cert;
    while (auto infoAccess = findAuthorityInformationAccess(curCert))
    {
        X509Certificate additionalCert = X509Certificate::from_der(HttpClient::request(infoAccess.ca_issuer()));
        data.ca.push(additionalCert);
        curCert = additionalCert;
    }

    Pkcs12::create(data, "").toDer();
    auto& ssl_instance = Instance::get();
    (void)ssl_instance;
}
