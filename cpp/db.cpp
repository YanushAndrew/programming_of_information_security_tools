#include "db.hpp"
#include <iostream>

DB::DB() {
    try {
        m_connection = new pqxx::connection("postgresql://user:password@localhost:5432/mydatabase");
        if (m_connection->is_open()) {
            std::cout << "Opened database successfully: " << m_connection->dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

DB::~DB() {
    if (m_connection->is_open()) {
    }
    delete m_connection;
}

std::string DB::getAddress(const std::string& login) {
    try {
        if (!m_connection || !m_connection->is_open()) {
            std::cerr << "Database connection is not open." << std::endl;
            return "";
        }

        m_connection->prepare("find_address", "SELECT address FROM users WHERE login = $1");

        pqxx::work W(*m_connection);
        pqxx::result R = W.exec(pqxx::prepped("find_address"), login);

        if (R.empty()) {
            return ""; 
        }

        return R[0][0].as<std::string>();

    } catch (const std::exception &e) {
        std::cerr << "Error getting address: " << e.what() << std::endl;
        return "";
    }
}

bool DB::authenticate(const std::string& login, const std::string& password) {
    try {
        if (!m_connection || !m_connection->is_open()) {
            std::cerr << "Database connection is not open." << std::endl;
            return false;
        }

        m_connection->prepare("authenticate_user", "SELECT password FROM users WHERE login = $1");

        pqxx::work W(*m_connection);
        pqxx::result R = W.exec(pqxx::prepped("authenticate_user"), login);

        if (R.empty()) {
            return false;
        }

        std::string stored_password = R[0][0].as<std::string>();
        return stored_password == password;

    } catch (const std::exception &e) {
        std::cerr << "Error during authentication: " << e.what() << std::endl;
        return false;
    }
}
