#include "parsingjob.h"
#include "MyJob.h"
#include <iostream>
#include <regex>
#include <sstream>
//#include <string>
//#include <vector>
#include "MyJobSystem.h"
#include "MyJobJSON.h"


ParsingJob::ParsingJob(unsigned int id, std::string ingest) : ingest(ingest) {
    this->id = id;
}

void ParsingJob::execute() {
    std::regex linker_txt_expr(
            "\\(.\\w+\\+0x\\w+\\): undefined reference to `\\w+'");
    std::regex linker_expr("clang-\\d+: error: (.*)");
    std::regex compiler_expr("(.*):(\\d+):(\\d+): (?:error|warning): (.*)");
    std::istringstream ess(this->ingest);
    std::string line;
    std::string linker_errs;
    while (std::getline(ess, line)) {
        std::smatch match;
        if (std::regex_match(line, match, compiler_expr)) {
            // Assumed that errors will have a snippet or extra line, so wait for 2
            // passes to add error
            std::string filename = match[1];
            Error error = {
                    .line = std::stoi(match[2]),
                    .column = std::stoi(match[3]),
                    .message = match[4],
            };
            if (!error.message.empty()) {
                auto error_vec = this->errors.find(filename);
                if (error_vec == this->errors.end()) {
                    this->errors.insert({filename, {}});
                    error_vec = this->errors.find(filename);
                }
                error_vec->second.push_back(error);
            }
        }
        if (std::regex_match(line, match, linker_txt_expr)) {
            linker_errs.append(line + '\n');
        }
        if (std::regex_match(line, match, linker_expr)) {
            Error error{
                    .line = 0,
                    .column = 0,
                    .message = linker_errs,
            };
            std::string filename = "/dev/null";
            auto error_vec = this->errors.find(filename);
            if (error_vec == this->errors.end()) {
                this->errors.insert({filename, {}});
                error_vec = this->errors.find(filename);
            }
            error_vec->second.push_back(error);
        }
    }
}

void ParsingJob::chain_next(JobSystem *system) {
    JSONJob *job = new JSONJob(JobSystem::NEXT_JOB_ID++, &this->errors);
    system->enqueue(job);
}
