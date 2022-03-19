#include "cosine_prod_cons_lib.h"

namespace po = boost::program_options;

void csc::parse_cli_args(int nargs, char **args, std::string &file_path, std::map<std::string, int> &threads,
                         long long& iterations) {
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")(
            "output,o", po::value<std::string>(), "output file to write results")(
            "producers,p", po::value<int>(), "number of producer threads to use")(
            "consumers,c", po::value<int>(), "number of consumer threads to use")(
            "iterations,i", po::value<long long>(), "specify the number of iterations for producing work");
    po::variables_map vm;
    po::store(po::parse_command_line(nargs, args, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        std::exit(0);
    }
    int cpu_threads = sysconf(_SC_NPROCESSORS_ONLN);
    file_path = vm.count("output") ? vm["output"].as<std::string>() : std::string("results.csv");
    iterations = vm.count("iterations") ? vm["iterations"].as<long long>() : MAX_ITERATIONS;
    threads.insert({"producers", vm.count("producers") ? vm["producers"].as<int>() : cpu_threads});
    threads.insert({"consumers", vm.count("consumers") ? vm["consumers"].as<int>() : cpu_threads});
}
