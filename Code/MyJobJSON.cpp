#include "MyJobJSON.h"
#include "parsingjob.h"
//#include <algorithm>
#include <fcntl.h>
//#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
//#include <vector>

JSONJob::JSONJob(
        unsigned int id,
        std::unordered_map<std::string, std::vector<ParsingJob::Error>> *errors)
        : errors(errors) {
    this->id = id;
}

std::vector<std::string> getLines(const std::string &filename, int lineNumber) {
    int fd = open(filename.c_str(), O_RDONLY);
    struct stat sb;
    int ret = fstat(fd, &sb);
    if (ret == -1) {
        return {};
    }
    char *file_mapped =
            (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_mapped == MAP_FAILED) {
        return {};
    }
    std::vector<std::string> result;
    char *start = file_mapped, *end;
    int lineCount = 0;
    for (end = start; end != file_mapped + sb.st_size; ++end) {
        if (*end == '\n') {
            if (lineCount >= lineNumber - 3 && lineCount < lineNumber + 2) {
                std::string line(start, end);
                if (!line.empty())
                    result.push_back(line);
            }
            start = end + 1;
            ++lineCount;
        }
    }
    if (end != start && lineCount >= lineNumber - 3 &&
        lineCount < lineNumber + 2) {
        std::string line(start, end);
        if (!line.empty())
            result.push_back(line);
    }
    munmap(file_mapped, sb.st_size);
    close(fd);
    return result;
}

void JSONJob::execute() {
    for (const std::pair<const std::string, std::vector<ParsingJob::Error>>
                &pair : *this->errors) {
        std::string filename = pair.first;
        for (const ParsingJob::Error &error : pair.second) {
            nlohmann::json entry;
            entry["line"] = error.line;
            entry["column"] = error.column;
            entry["message"] = error.message;
            entry["chunk"] = getLines(filename, error.line);
            this->errors_json[filename].push_back(entry);
        }
    }
}
