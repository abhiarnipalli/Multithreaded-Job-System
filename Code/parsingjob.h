#ifndef LAB_1_MULTITHREADING_ABHIARNIPALLI_PARSINGJOB_H
#define LAB_1_MULTITHREADING_ABHIARNIPALLI_PARSINGJOB_H
#include "MyJob.h"
#include <string>
#include <unordered_map>
#include <vector>

class ParsingJob : public Job {
private:
    std::string ingest;

public:
    struct Error {
        std::string filename;
        int line, column;
        std::string message;
    };
    std::unordered_map<std::string, std::vector<ParsingJob::Error>> errors;
    unsigned int id;
    ParsingJob(unsigned int id, std::string ingest);
    void execute() override;
    void chain_next(JobSystem *system) override;
};

#endif //LAB_1_MULTITHREADING_ABHIARNIPALLI_PARSINGJOB_H
