#include "lib/cosine_prod_cons_lib.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        std::string file_path{};
        std::map<std::string, int> threads{};
        long long iterations{};
        csc::parse_cli_args(argc, argv, file_path, threads, iterations);
        std::vector<pthread_t> producers = std::vector<pthread_t>(threads.at("producers"));
        std::vector<pthread_t> consumers = std::vector<pthread_t>(threads.at("consumers"));
        csc::queue *queue;
        queue = csc::queueInit(threads.at("producers"), threads.at("consumers"), iterations);

        if (queue ==  nullptr) {
            std::cerr <<  "Main: Queue init failed." << std::endl;
            exit (1);
        }
        rc::Document doc;
        csc::createFile(doc);
        queue->document = &doc;
        std::cout << "got here" <<std::endl;
        for(auto item: producers){
            if(pthread_create(&item, nullptr, csc::producer, queue)){
                std::cerr << "Failed to create new thread in producer" << std::endl;
                exit(-1);
            }
            pthread_join(item, nullptr);

        }
        for(auto item: consumers){
            if(pthread_create(&item, nullptr, csc::consumer, queue)){
                std::cerr << "Failed to create new thread in producer" << std::endl;
                exit(-1);
            }
            pthread_join(item, nullptr);
        }
        csc::closeFile(file_path.c_str(), doc);
        csc::queueDelete(queue);
    }
    return 0;
}
