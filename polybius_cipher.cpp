// polybius_cipher.cpp
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;

// Global Polybius square and coordinate maps for simplicity
char polybiusSquare[5][5] =
{
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'K'},
    {'L', 'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z'}
    // i removed J because it's the same as I, in my case
};

map<char, pair<int, int>> charToCoord;
map<pair<int, int>, char> coordToChar;

void initializePolybius()
{
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            char c = polybiusSquare[i][j];
            charToCoord[c] = {i + 1, j + 1};
            coordToChar[{i + 1, j + 1}] = c;
            if (c == 'I')
            { // Handle J as I
                charToCoord['J'] = {i + 1, j + 1};
            }
        }
    }
}

// Preprocesses text to keep only alphabetic characters and convert to uppercase
string preprocessText(string text)
{
    string processedText;
    for (char c : text)
    {
        if (isalpha(c))
        {
            processedText += toupper(c);
        }
    }
    return processedText;
}

// Method 1: Substitute with the letter below in the same column
string polybiusEncryptMethod1(const string& plaintext)
{
    string ciphertext;
    for (char c : plaintext)
    {
        if (charToCoord.count(c))
        {
            pair<int, int> coords = charToCoord[c];
            int newRow = (coords.first % 5) + 1; // Wrap around from row 5 to 1
            ciphertext += coordToChar[{newRow, coords.second}];
        }
    }
    return ciphertext;
}

// Method 2: Rearrange coordinates
string polybiusEncryptMethod2(const string& plaintext)
{
    string rows, cols;
    for (char c : plaintext)
    {
        if (charToCoord.count(c))
        {
            pair<int, int> coords = charToCoord[c];
            rows += to_string(coords.first);
            cols += to_string(coords.second);
        }
    }

    string combinedCoords = cols + rows;
    string ciphertext;
    for (size_t i = 0; i < combinedCoords.length(); i += 2)
    {
        // Pairs are treated as (col, row)
        int col = combinedCoords[i] - '0';
        int row = combinedCoords[i+1] - '0';
        ciphertext += coordToChar[{row, col}];
    }
    return ciphertext;
}

// Method 3: Rearrange and shift coordinates
string polybiusEncryptMethod3(const string& plaintext)
{
    string rows, cols;
    for (char c : plaintext)
    {
        if (charToCoord.count(c))
        {
            pair<int, int> coords = charToCoord[c];
            rows += to_string(coords.first);
            cols += to_string(coords.second);
        }
    }
    
    string combinedCoords = cols + rows;
    // Cyclic shift to the left by one position
    rotate(combinedCoords.begin(), combinedCoords.begin() + 1, combinedCoords.end());
    
    string ciphertext;
    for (size_t i = 0; i < combinedCoords.length(); i += 2)
    {
        // Pairs are treated as (col, row)
        int col = combinedCoords[i] - '0';
        int row = combinedCoords[i+1] - '0';
        ciphertext += coordToChar[{row, col}];
    }
    return ciphertext;
}

int main()
{
    initializePolybius();

    string plaintext = "Yanush";
    string processedText = preprocessText(plaintext);
    
    cout << "Original Plaintext: " << plaintext << "\n";
    cout << "Processed Plaintext (uppercase): " << processedText << "\n";
    cout << "\n";

    string ciphertext1 = polybiusEncryptMethod1(processedText);
    cout << "Method 1 Ciphertext: " << ciphertext1 << "\n";

    string ciphertext2 = polybiusEncryptMethod2(processedText);
    cout << "Method 2 Ciphertext: " << ciphertext2 << "\n";

    string ciphertext3 = polybiusEncryptMethod3(processedText);
    cout << "Method 3 Ciphertext: " << ciphertext3 << "\n";

    return 0;
}
