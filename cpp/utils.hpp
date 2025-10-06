#pragma once

#include <string>

/**
 * @brief Gets the MAC address of the first available network interface.
 * 
 * This function is cross-platform and supports Linux, Windows, and macOS.
 * @return The MAC address as a string (e.g., "00:1A:2B:3C:4D:5E"), or an empty string on failure.
 */
std::string getMacAddress();

/**
 * @brief Gets the current date.
 * 
 * @return The current date as a string in "YYYY-MM-DD" format.
 */
std::string getCurrentDate();

/**
 * @brief Calculates the number of days between two dates.
 * 
 * @param date1_str The first date as a string in "YYYY-MM-DD" format.
 * @param date2_str The second date as a string in "YYYY-MM-DD" format.
 * @return The number of days between the two dates.
 */
long days_between(const std::string& date1_str, const std::string& date2_str);
