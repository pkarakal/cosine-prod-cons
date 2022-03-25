#ifndef PROD_CONS_COSINE_PROD_CONS_LIB_H
#define PROD_CONS_COSINE_PROD_CONS_LIB_H

#include <algorithm>
#include <boost/program_options.hpp>
#include <chrono>
#include <cmath>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <pthread.h>
#include <random>
#include <string>
#include <unistd.h>
#include <vector>
#include "definitions.h"
#include "rapidcsv.hpp"

#define MAX_ITERATIONS 10'000'000'000

namespace rc = rapidcsv;

namespace csc {
    void parse_cli_args(int nargs, char **args, std::string &file_path, std::map<std::string, int> &threads,
                        long long &iterations);

    void *producer(void *args);

    void *consumer(void *args);

    typedef struct {
        void *(*work)(void *);
        void *arg;
        long long iterations;
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> add_time;
    } workFunction;

    typedef struct {
        std::deque<workFunction> buf;
        long head, tail;
        bool full, empty;
        pthread_mutex_t *mut;
        pthread_cond_t *notFull, *notEmpty;
        long long producers, consumers;
        rc::Document *document;
    } queue;

    queue *queueInit(long long producers, long long consumers, long long iterations);

    void queueDelete(queue *q);

    void queueAdd(queue *q, workFunction *in);

    void queueDel(queue *q, workFunction **out);

    void *calc_cosine(long long angle);

    void createFile(rc::Document& document);

    void writeResultToFile(rc::Document& file, double time);

    void closeFile(const char* file_name, rc::Document& file);

}

#endif //PROD_CONS_COSINE_PROD_CONS_LIB_H
