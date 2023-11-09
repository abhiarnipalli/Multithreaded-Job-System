#ifndef LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBSYSTEM_H
#define LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBSYSTEM_H
#include "slave.h"
#include <vector>

enum JobStatus { NEVER_SEEN, QUEUED, RUNNING, COMPLETED, N_JOB_STATUSES };
struct HistoryEntry {
public:
    int id = 0;
    JobStatus status;
    HistoryEntry(int id = -1, JobStatus status = JobStatus::NEVER_SEEN)
            : id(id), status(status) {}
};

class JobSystem {
    friend Slave;
public:
    static std::atomic<unsigned long> NEXT_JOB_ID;
    using Message = Job *;
    void enqueue(Job *job);
    JobStatus get_status(unsigned long id) const;
    void remove_entry(unsigned long id);
    void add_slave(std::string name);
    std::vector<Job *> get_completed(unsigned long n_jobs);
    std::vector<HistoryEntry> current_history() const;
    void join();
private:
    std::vector<Slave> slaves;
    MessageQueue<Slave::Message, std::priority_queue, Slave::CompareMessage>
            queued_jobs;
    MessageQueue<JobSystem::Message, std::queue> completed_jobs;
    std::vector<HistoryEntry> history;
    mutable std::mutex history_mtx;
    void update_id_history(Job *job, JobStatus status);
    void transition_to_completed(Job *job);
};

#endif //LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBSYSTEM_H
