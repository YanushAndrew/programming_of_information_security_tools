#pragma once

#include <string>
#include <vector>

class File {
public:
    File(const std::string& filename);
    void addRecord(const std::string& record);
    std::vector<std::string> filterBy(const std::string& searchTerm);

private:
    std::string m_filename;
};
