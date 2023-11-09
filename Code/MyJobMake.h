#ifndef LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBMAKE_H
#define LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBMAKE_H
#include "MyJob.h"
#include <string>

class MakeJob : public Job {
public:
    std::string target;
    std::string stdout;
    unsigned char retcode;
    MakeJob(unsigned int id, std::string target);
    void execute() override;
    void chain_next(JobSystem *system) override;
};
#endif //LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBMAKE_H

