
#ifndef LAB_1_MULTITHREADING_ABHIARNIPALLI_SLAVE_H
#define LAB_1_MULTITHREADING_ABHIARNIPALLI_SLAVE_H
#include <queue>
#include <string>
#include <thread>
#include <variant>
#include "MyJob.h"
#include "MyJobQueue.h"

class JobSystem;
class Slave {
    friend JobSystem;
public:
    struct NewJob {
        Job *job;
    };
    struct Exit {};
    using Message = std::variant<NewJob, Exit>;
    struct CompareMessage {
        /// Operator gives Exit less priority
        bool operator()(const Message &a, Message &b) {
            return std::holds_alternative<Exit>(a) &&
                   std::holds_alternative<NewJob>(b);
        }
    };
    Slave(std::string name,
          MessageQueue<Slave::Message, std::priority_queue, Slave::CompareMessage>
          *job_waiting_mq,
          JobSystem *system);
private:
    std::string name;
    std::thread handle;
    JobSystem *system;
    void WorkDaemon();
};


#endif //LAB_1_MULTITHREADING_ABHIARNIPALLI_SLAVE_H
