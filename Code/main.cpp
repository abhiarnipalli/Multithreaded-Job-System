#include "MyJobSystem.h"
#include "MyJobJSON.h"
#include "MyJobMake.h"
#include "parsingjob.h"
#include <iostream>

int main(int argc, char *argv[]) {
    std::vector<MakeJob *> loadablejobs;
    if (argc == 1) {
        std::cerr << "No targets: Re-run: "
                  << argv[0] << " target" <<std::endl;
        return 1;
    }
    else {
        for (int n = 1; n < argc; ++n) {
            MakeJob *mj = new MakeJob(0, argv[n]);
            loadablejobs.push_back(mj);
        }
    }
    JobSystem js;
    for (unsigned int n = 1; n <= std::thread::hardware_concurrency(); ++n)
        js.add_slave("thread" + std::to_string(n));
    for (MakeJob *job : loadablejobs) {
        js.enqueue(job);
    }
    std::vector<Job *> completed_jobs = js.get_completed(3 * loadablejobs.size());
    for (Job *job : completed_jobs) {
        JSONJob *jj = dynamic_cast<JSONJob *>(job);
        if (jj != nullptr) {
            std::cout << jj->errors_json;
        }
        delete job;
    }
    js.join();
    return 0;
}