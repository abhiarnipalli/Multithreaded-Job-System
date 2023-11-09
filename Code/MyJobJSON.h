#ifndef LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBJSON_H
#define LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBJSON_H
#include "MyJob.h"
#include "parsingjob.h"
#include "json.h"
#include "jsonMove.h"

class JSONJob : public Job {
private:
    std::unordered_map<std::string, std::vector<ParsingJob::Error>> *errors;
public:
    nlohmann::json errors_json;
    JSONJob(
            unsigned int id,
            std::unordered_map<std::string, std::vector<ParsingJob::Error>> *errors);
    void execute() override;
};

#endif //LAB_1_MULTITHREADING_ABHIARNIPALLI_MYJOBJSON_H
