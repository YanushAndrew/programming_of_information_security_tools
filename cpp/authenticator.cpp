#include "authenticator.hpp"
#include "utils.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <dirent.h>
#include <cstring>
#include <fstream> // Added for file operations

// Helper function to get a future date
std::string getDateInFuture(int days) {
    auto now = std::chrono::system_clock::now();
    auto future_time = now + std::chrono::hours(24 * days);
    auto in_time_t = std::chrono::system_clock::to_time_t(future_time);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

// Helper function to parse a date string
std::chrono::system_clock::time_point parseDate(const std::string& date_str) {
    std::tm tm = {};
    std::stringstream ss(date_str);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

Authenticator::Authenticator(const std::string& banListFilename, const std::string& attemptsFilename)
    : m_banListFile(banListFilename), m_attemptsFile(attemptsFilename) {}

void Authenticator::logFailedAttempt() {
    std::string mac = getMacAddress(); // Now calls the global, cross-platform function
    if (!mac.empty()) {
        std::string record = getCurrentDate() + "," + mac;
        m_attemptsFile.addRecord(record);
    } else {
        std::cerr << "Could not log failed attempt: MAC address could not be retrieved." << std::endl;
    }
}

void Authenticator::banMacAddress(const std::string& mac, int days) {
    int ban_days = days;
    if (ban_days < 1) {
        std::cout << "Warning: Ban duration cannot be less than 1 day. Defaulting to 1 day." << std::endl;
        ban_days = 1;
    }

    // First, check if the MAC is already banned to avoid duplicate entries
    if (isMacBanned(mac)) {
        std::cout << "MAC address " << mac << " is already banned." << std::endl;
        return;
    }
    std::string expirationDate = getDateInFuture(ban_days);
    std::string record = mac + "," + expirationDate;
    m_banListFile.addRecord(record);
}

bool Authenticator::isMacBanned(const std::string& mac) {
    auto bannedUsers = m_banListFile.filterBy(mac);
    if (bannedUsers.empty()) {
        return false;
    }

    std::string currentDateStr = getCurrentDate();
    auto now = parseDate(currentDateStr);

    for (const auto& record : bannedUsers) {
        std::stringstream ss(record);
        std::string recordMac, expirationDateStr;
        if (std::getline(ss, recordMac, ',') && std::getline(ss, expirationDateStr)) {
            if (recordMac == mac) {
                auto expirationDate = parseDate(expirationDateStr);
                if (now <= expirationDate) {
                    return true; // Ban is active
                }
            }
        }
    }
    return false;
}

std::vector<std::string> Authenticator::analyzeFailedAttempts() {
    auto allAttempts = m_attemptsFile.filterBy(""); // Get all records
    
    // In a real-world scenario, reading the whole file might be inefficient.
    // For this case, we'll process it in memory. We'll take the last 200.
    int start = std::max(0, (int)allAttempts.size() - 200);
    std::vector<std::string> recentAttempts(allAttempts.begin() + start, allAttempts.end());

    std::unordered_map<std::string, std::vector<std::chrono::system_clock::time_point>> macAttempts;
    auto thirtyDaysAgo = std::chrono::system_clock::now() - std::chrono::hours(24 * 30);

    for (const auto& record : recentAttempts) {
        std::stringstream ss(record);
        std::string dateStr, mac;
        if (std::getline(ss, dateStr, ',') && std::getline(ss, mac)) {
            auto attemptTime = parseDate(dateStr);
            if (attemptTime >= thirtyDaysAgo) {
                macAttempts[mac].push_back(attemptTime);
            }
        }
    }

    std::vector<std::string> macsToBan;
    for (const auto& pair : macAttempts) {
        if (pair.second.size() >= 12) {
            macsToBan.push_back(pair.first);
        }
    }

    return macsToBan;
}

void Authenticator::banSuspiciousMacs(int days) {
    auto macsToBan = analyzeFailedAttempts();
    for (const auto& mac : macsToBan) {
        std::cout << "Banning suspicious MAC: " << mac << " for " << days << " days." << std::endl;
        banMacAddress(mac, days);
    }
}

bool Authenticator::hasExceededDailyAttempts(const std::string& mac, int maxAttempts) {
    auto allAttemptsForMac = m_attemptsFile.filterBy(mac);
    
    std::string today = getCurrentDate();
    int todaysAttempts = 0;

    for (const auto& record : allAttemptsForMac) {
        std::stringstream ss(record);
        std::string dateStr, recordMac;
        
        if (std::getline(ss, dateStr, ',') && std::getline(ss, recordMac)) {
            if (recordMac == mac && dateStr == today) {
                todaysAttempts++;
            }
        }
    }

    return todaysAttempts > maxAttempts;
}

std::string Authenticator::getBanExpirationDate(const std::string& mac) {
    auto bannedUsers = m_banListFile.filterBy(mac);
    if (bannedUsers.empty()) {
        return "";
    }

    auto now = parseDate(getCurrentDate());

    for (const auto& record : bannedUsers) {
        std::stringstream ss(record);
        std::string recordMac, expirationDateStr;
        if (std::getline(ss, recordMac, ',') && std::getline(ss, expirationDateStr)) {
            if (recordMac == mac) {
                auto expirationDate = parseDate(expirationDateStr);
                if (now <= expirationDate) {
                    return expirationDateStr; // Ban is active, return the date
                }
            }
        }
    }
    return ""; // No active ban found
}
