// vernam_cipher.cpp
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>

using namespace std;

string generateKey(int length)
{
    string key;
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    
    mt19937 rng(time(nullptr));
    uniform_int_distribution<int> dist(0, sizeof(charset) - 2);

    for (int i = 0; i < length; ++i)
    {
        key += charset[dist(rng)];
    }
    return key;
}

string vernamEncrypt(
    const string& message,
    const string& key)
{
    if (message.length() != key.length())
    {
        cerr << "Error: Message and key must have the same length." << endl;
        return "";
    }

    string ciphertext;
    for (size_t i = 0; i < message.length(); ++i)
    {
        ciphertext += message[i] ^ key[i];
    }
    return ciphertext;
}

string byteToHex(const string& bytes)
{
    stringstream ss;
    for(char c : bytes)
    {
        ss << hex << setfill('0') << setw(2) << (int)(unsigned char)c;
    }
    return ss.str();
}

string hexToByte(const string& h_hex)
{
    string bytes;
    for (size_t i = 0; i < h_hex.length(); i += 2)
    {
        std::string byteString = h_hex.substr(i, 2);
        int byteValue = std::stoi(byteString, nullptr, 16);
        bytes += static_cast<char>(byteValue);
    }
    return bytes;
}

int main() {
    string plaintext = "Yanush";
    cout << "Plaintext: " << plaintext << "\n";

    // Generate a random key with the same length as the plaintext
    string key = generateKey(plaintext.length());
    cout << "Generated Key: " << key << "\n";

    // Encrypt the plaintext
    string ciphertext = vernamEncrypt(plaintext, key);
    cout << "Ciphertext (raw): " << ciphertext << "\n"; //crypted shown as post-hex values, so it won't(most likely) match the original with symbol amount,
    // upd: because there are unprintable characters in the hex output, so they are missing
    // for example: 02 07
    
    cout << "hex: "<< byteToHex(ciphertext) << "\n"; // with this format we can store crypted data

    cout << vernamEncrypt(ciphertext, key) << "\n";
    cout << vernamEncrypt(hexToByte(byteToHex(ciphertext)), key) << "\n";

    return 0;
}
