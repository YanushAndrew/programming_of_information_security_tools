#include <iostream>
#include <string>
#include <chrono>
#include "db.hpp"
#include "authenticator.hpp"
#include "utils.hpp"

int main() {
    DB db;
    Authenticator auth("ban_list.txt", "failed_attempts.txt");
    int request_count = 0;

    while (true) {
        request_count++;
        std::cout << "\n--- New Request (" << request_count << ") ---\n";

        // Step 7: On the 20th request, run the deep analysis
        if (request_count > 0 && request_count % 20 == 0) {
            std::cout << "[SYSTEM] Analyzing failed attempts for suspicious activity...\n";
            auth.banSuspiciousMacs(30);
        }

        std::string mac = getMacAddress();
        if (mac.empty()) {
            std::cerr << "Fatal Error: Could not retrieve device MAC address. Cannot proceed.\n";
            return 1;
        }

        // Step 5: Check if the user is already in the ban list
        if (auth.isMacBanned(mac)) {
            std::string ban_until = auth.getBanExpirationDate(mac);
            long days_left = days_between(getCurrentDate(), ban_until);
            // Step 6 [ban condition]
            std::cout << "[ACCESS DENIED] Your device is currently in the ban list.\n";
            std::cout << "Ban active until: " << ban_until << " (" << days_left << " days left).\n";
            break; // Terminate the program for a banned user
        }

        // Step 2 & 3 & 4: Prompt for credentials
        std::cout << "Enter your credentials to receive your private data\n";
        std::string login, password;
        std::cout << "Login: ";
        std::cin >> login;
        std::cout << "Password: ";
        std::cin >> password;

        if (db.authenticate(login, password)) {
            // Step 6 [right credentials]
            std::string address = db.getAddress(login);
            std::cout << "\n[SUCCESS] Authentication successful.\n";
            std::cout << "Your private data (address): " << address << "\n";
        } else {
            // Step 6 [wrong credentials]
            std::cout << "\n[ERROR] Authentication failed: Incorrect login or password.\n";
            auth.logFailedAttempt();

            // Step 6 [third wrong attempt]
            if (auth.hasExceededDailyAttempts(mac, 2)) {
                std::cout << "[SECURITY ALERT] Too many failed attempts from your device today.\n";
                std::cout << "Banning for 1 day.\n";
                auth.banMacAddress(mac, 1);
                std::cout << "The application will now terminate for this device.\n";
                break; // End the program for this user
            } else {
                std::cout << "Please try again.\n";
            }
        }
    }

    return 0;
}