#include "cosine_prod_cons_lib.h"

namespace po = boost::program_options;

void csc::parse_cli_args(int nargs, char **args, std::string &file_path, std::map<std::string, int> &threads,
                         long long &iterations) {
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
    long cpu_threads = sysconf(_SC_NPROCESSORS_ONLN);
    file_path = vm.count("output") ? vm["output"].as<std::string>() : std::string("results.csv");
    iterations = vm.count("iterations") ? vm["iterations"].as<long long>() : MAX_ITERATIONS;
    threads.insert({"producers", vm.count("producers") ? vm["producers"].as<int>() : cpu_threads});
    threads.insert({"consumers", vm.count("consumers") ? vm["consumers"].as<int>() : cpu_threads});
}

void *csc::producer(void *q) {
    csc::queue *fifo;

    fifo = (queue *) q;

    for (int i = 0; i < fifo->producers; i++) {
        auto *_func = (csc::workFunction *)malloc(sizeof(csc::workFunction));
        _func->work = reinterpret_cast<void *(*)(void *)>(csc::calc_cosine);
        _func->arg = malloc(sizeof(long long));
        pthread_mutex_lock(fifo->mut);
        while (fifo->full) {
            pthread_cond_wait(fifo->notFull, fifo->mut);
        }
        *((long long *)_func->arg) = i;
        csc::queueAdd(fifo, _func);
        pthread_mutex_unlock(fifo->mut);
        pthread_cond_signal(fifo->notEmpty);
    }
    return nullptr;
}

void *csc::consumer(void *q) {
    csc::queue *fifo;
    fifo = (queue *) q;
    auto cons = 0;
    while(true){
        if(cons == fifo->producers){
            break;
        }
        csc::workFunction *func;
        pthread_mutex_lock(fifo->mut);
        while (fifo->empty) {
            pthread_cond_wait(fifo->notEmpty, fifo->mut);
        }
        csc::queueDel(fifo, &func);
        (*((void(*)(long long))func->work))(*((long long*) func->arg));
        pthread_mutex_unlock(fifo->mut);
        pthread_cond_signal(fifo->notFull);
        cons++;
    }
    return nullptr;
}


csc::queue *csc::queueInit(long long producers, long long consumers, long long iterations) {
    csc::queue *q;

    q = (queue *) malloc(sizeof(queue));
    if (q == nullptr) return (nullptr);

    q->buf = std::deque<csc::workFunction>(iterations);
    q->producers = producers;
    q->consumers = consumers;

    q->empty = true;
    q->full = false;
    q->head = 0;
    q->tail = 0;
    q->mut = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(q->mut, nullptr);
    q->notFull = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notFull, nullptr);
    q->notEmpty = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notEmpty, nullptr);

    return (q);
}

void csc::queueDelete(queue *q) {
    pthread_mutex_destroy(q->mut);
    free(q->mut);
    pthread_cond_destroy(q->notFull);
    free(q->notFull);
    pthread_cond_destroy(q->notEmpty);
    free(q->notEmpty);
    free(q);
}

void csc::queueAdd(queue *q, csc::workFunction *in) {
    in->add_time = std::chrono::high_resolution_clock::now();
    q->buf.insert(q->buf.begin(), *in);
    q->tail++;
    if (q->tail == q->buf.size())
        q->tail = 0;
    if (q->tail == q->head)
        q->full = true;
    q->empty = false;
}

void csc::queueDel(csc::queue *q, csc::workFunction **out) {
    if(q->buf.empty()){
        return;
    }
    *out = &q->buf.front();
    q->buf.pop_front();
    auto pop_time = std::chrono::high_resolution_clock::now();
    auto duration = (pop_time - (*out)->add_time).count() * 10e-9;
    csc::writeResultToFile(*(q->document), duration);

    std::cout << duration <<std::endl;

    q->head++;
    if (q->head == q->buf.size())
        q->head = 0;
    if (q->head == q->tail)
        q->empty = true;
    q->full = false;
}

void *csc::calc_cosine(long long angle){
    std::cout << angle << "," << std::cos(angle) << std::endl;
    return nullptr;
}

void csc::createFile(rc::Document& document){
    document = rc::Document("", rc::LabelParams(-1, -1));
    std::vector<std::string> headers {"index, time in queue"};
    document.SetRow(0, headers);
}


void csc::writeResultToFile(rc::Document& file, double duration){
    std::vector<std::string> tmp{ std::to_string(file.GetRowCount()+1), std::to_string(duration)};
    file.SetRow(file.GetRowCount(), tmp);
}

void csc::closeFile(const char* file_name, rc::Document& file){
    file.Save(std::string(file_name));
}