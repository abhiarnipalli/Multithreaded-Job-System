#ifndef LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOB_H
#define LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOB_H
#include <atomic>

class JobSystem;
class Job {
public:
    unsigned int id;
    virtual void execute() = 0;
    virtual void chain_next(JobSystem *system){};
    virtual ~Job(){};
};

#endif //LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOB_H
