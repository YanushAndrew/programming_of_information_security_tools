#include "file.hpp"
#include <fstream>
#include <iostream>
#include <vector>

File::File(const std::string& filename) : m_filename(filename) {
    std::ofstream file(m_filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error creating or opening the file." << std::endl;
    }
}

void File::addRecord(const std::string& record) {
    std::ofstream file(m_filename, std::ios::app);
    if (file.is_open()) {
        file << record << std::endl;
    } else {
        std::cerr << "Error opening the file to add a record." << std::endl;
    }
}

std::vector<std::string> File::filterBy(const std::string& searchTerm) {
    std::vector<std::string> results;
    std::ifstream file(m_filename);
    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find(searchTerm) != std::string::npos) {
                results.push_back(line);
            }
        }
    } else {
        std::cerr << "Error opening the file for filtering." << std::endl;
    }

    return results;
}