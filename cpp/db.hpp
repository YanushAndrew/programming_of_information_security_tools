#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <pqxx/pqxx>

class DB {
    public:
        DB();
        ~DB();
        std::string getAddress(const std::string& login);
        bool authenticate(const std::string& login, const std::string& password);
    private:
        pqxx::connection* m_connection;
};