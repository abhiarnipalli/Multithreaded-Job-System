#include "MyJobSystem.h"
#include <algorithm>
#include <iostream>
#include <mutex>

std::atomic<unsigned long> JobSystem::NEXT_JOB_ID = 0;
void JobSystem::enqueue(Job *job) {
    HistoryEntry entry = HistoryEntry(job->id, JobStatus::QUEUED);
    std::unique_lock<std::mutex> history_lock(this->history_mtx);
    this->history.push_back(entry);
    history_lock.unlock();
    Slave::Message message = Slave::NewJob{.job = job};
    this->queued_jobs.send(message);
}

JobStatus JobSystem::get_status(unsigned long id) const {
    auto entry = std::find_if(this->history.begin(), this->history.begin(),
                              [id](HistoryEntry entry) { return entry.id == id; });
    if (entry != this->history.end()) {
        return entry->status;
    } else {
        return JobStatus::NEVER_SEEN;

    }
}

void JobSystem::remove_entry(unsigned long id) {
    std::lock_guard<std::mutex> history_lock(this->history_mtx);
    (void)std::remove_if(this->history.begin(), this->history.end(),
                         [id](HistoryEntry entry) { return entry.id == id; });
}

std::vector<Job *> JobSystem::get_completed(unsigned long n_jobs) {
    return this->completed_jobs.receive_n(n_jobs);
}

void JobSystem::update_id_history(Job *job, JobStatus status) {
    std::lock_guard<std::mutex> history_lock(this->history_mtx);
    HistoryEntry *found_entry = nullptr;
    for (HistoryEntry entry : this->history) {
        if (entry.id == job->id) {
            found_entry = &entry;
        }
        found_entry->status = status;
    }
}

std::vector<HistoryEntry> JobSystem::current_history() const {
    std::lock_guard<std::mutex> history_lock(this->history_mtx);
    return this->history;
}

void JobSystem::join() {
    for (unsigned long n = 0; n < this->slaves.size(); ++n)
        this->queued_jobs.send(Slave::Exit{});
    for (auto slave_it = slaves.begin(); slave_it != slaves.end(); ++slave_it)
        slave_it->handle.join();
    if (!this->queued_jobs.empty())
        std::cerr << "Warning:";
}

void JobSystem::add_slave(std::string name) {
    this->slaves.emplace_back(name, &this->queued_jobs, this);
}
