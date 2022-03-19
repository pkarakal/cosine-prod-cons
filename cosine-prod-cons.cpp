#include "lib/cosine_prod_cons_lib.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        std::string file_path{};
        std::map<std::string, int> threads{};
        long long iterations{};
        csc::parse_cli_args(argc, argv, file_path, threads, iterations);
    }
    return 0;
}
