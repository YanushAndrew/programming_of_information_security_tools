#pragma once

#include "file.hpp"
#include <string>
#include <vector>

class Authenticator {
public:
    Authenticator(const std::string& banListFilename, const std::string& attemptsFilename);

    // 1. Function to ban a MAC address
    void banMacAddress(const std::string& mac, int days = 30);

    // 2. Function to check if a MAC is in the ban list
    bool isMacBanned(const std::string& mac);

    // 3. Function to log a failed attempt from the current machine
    void logFailedAttempt();

    // 4. Function to analyze attempts and find suspicious MACs
    std::vector<std::string> analyzeFailedAttempts();

    // 5. Function to ban suspicious MACs
    void banSuspiciousMacs(int days = 30);

    /**
     * @brief Checks if a given MAC address has exceeded a specific number of failed attempts for the current day.
     * @param mac The MAC address to check.
     * @param maxAttempts The maximum number of allowed attempts.
     * @return True if the attempt count for today is greater than maxAttempts, false otherwise.
     */
    bool hasExceededDailyAttempts(const std::string& mac, int maxAttempts);

    /**
     * @brief Gets the expiration date of a ban for a given MAC address.
     * @param mac The MAC address to check.
     * @return The expiration date as a string ("YYYY-MM-DD") if the ban is active, otherwise an empty string.
     */
    std::string getBanExpirationDate(const std::string& mac);

private:
    File m_banListFile;
    File m_attemptsFile;
};
