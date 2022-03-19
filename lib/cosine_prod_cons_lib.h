#ifndef PROD_CONS_COSINE_PROD_CONS_LIB_H
#define PROD_CONS_COSINE_PROD_CONS_LIB_H

#include <algorithm>
#include <boost/program_options.hpp>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <random>
#include <string>
#include <vector>
#include "definitions.h"

#define MAX_ITERATIONS 10'000'000'000

namespace csc {
    void parse_cli_args(int nargs, char **args, std::string &file_path, std::map<std::string, int> &threads,
                        long long& iterations);
}

#endif //PROD_CONS_COSINE_PROD_CONS_LIB_H
