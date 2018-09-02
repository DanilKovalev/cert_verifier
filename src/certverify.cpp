#include "Instance.h"
#include <boost/program_options.hpp>
#include <iostream>

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

int main(int argc, char** argv)
{
    po::variables_map vm = init_options(argc, argv);

    auto certPath =  vm["certificate"].as<std::string>();



    auto& ssl_instance = Instance::get();
    (void)ssl_instance;

}
