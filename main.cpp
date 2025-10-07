#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include "src/bbs.h"

// Function to convert a string to a vector of bytes
std::vector<char> string_to_bytes(const std::string& str) {
    return std::vector<char>(str.begin(), str.end());
}

// Function to convert a vector of bytes to a string
std::string bytes_to_string(const std::vector<char>& bytes) {
    return std::string(bytes.begin(), bytes.end());
}

// Function to perform XOR encryption/decryption
std::vector<char> xor_encrypt_decrypt(const std::vector<char>& data, BBS& bbs) {
    std::vector<char> result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        char key_byte = 0;
        for (int j = 0; j < 8; ++j) {
            key_byte = (key_byte << 1) | bbs.getRandBit();
        }
        result[i] = data[i] ^ key_byte;
    }
    return result;
}

int main() {
    // 1. Choose two three-digit Blum prime numbers p and q.
    // A Blum prime is a prime number of the form 4k+3.
    // Let's choose p = 499 and q = 547. Both are primes and 499 % 4 = 3, 547 % 4 = 3.
    long p = 499;
    long q = 547;

    // 2. Calculate n = p * q
    long n = p * q;

    // 3. Choose a random seed x, where x is a quadratic residue mod n.
    // For simplicity, we choose a seed and ensure it's not a multiple of p or q.
    long seed = 12345; 

    std::cout << "Using p = " << p << ", q = " << q << std::endl;
    std::cout << "n = " << n << std::endl;
    std::cout << "seed = " << seed << std::endl;

    // The message to be encrypted (your last name)
    std::string message = "Yanush"; // Please replace with your actual last name

    std::cout << "\nOriginal message: " << message << std::endl;

    // Encryption
    BBS bbs_encrypt(p, q, seed);
    std::vector<char> byte_message = string_to_bytes(message);
    std::vector<char> encrypted_bytes = xor_encrypt_decrypt(byte_message, bbs_encrypt);

    std::cout << "Encrypted (hex): ";
    for (char c : encrypted_bytes) {
        std::cout << std::hex << (static_cast<int>(c) & 0xff);
    }
    std::cout << std::dec << std::endl;

    // Decryption
    BBS bbs_decrypt(p, q, seed); // Re-initialize BBS with the same parameters
    std::vector<char> decrypted_bytes = xor_encrypt_decrypt(encrypted_bytes, bbs_decrypt);
    std::string decrypted_message = bytes_to_string(decrypted_bytes);

    std::cout << "Decrypted message: " << decrypted_message << std::endl;

    return 0;
}
