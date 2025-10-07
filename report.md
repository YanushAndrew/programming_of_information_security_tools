# Звіт з самостійної роботи

## а) Результати досліджень

В ході виконання даної самостійної роботи було досліджено алгоритм генерації псевдовипадкових чисел Блюма-Блюма-Шуба (BBS) та його застосування для шифрування даних.

**Параметри генератора BBS:**
- Два тризначних числа p та q, які є простими числами Блюма (p ≡ 3 (mod 4), q ≡ 3 (mod 4)).
  - Було обрано `p = 499` та `q = 547`.
- Модуль `n = p * q = 499 * 547 = 272953`.
- Випадкове число (seed) `x = 12345`.

Шифрування виконується шляхом побітового XOR кожного байта вихідного повідомлення з байтом, згенерованим послідовністю BBS.

## б) Протокол Ваших дій

1. Створено структуру проєкту з директорією `src` для вихідних кодів.
2. Використано готову реалізацію генератора BBS з GitHub (`bbs.h`, `bbs.cpp`).
3. Створено головний файл `main.cpp`, який реалізує логіку шифрування та розшифрування повідомлення.
4. У `main.cpp` було обрано параметри p, q та seed, а також повідомлення для шифрування (прізвище).
5. Створено `Makefile` для автоматизації компіляції проєкту.
6. Скомпільовано та запущено програму для перевірки коректності шифрування та розшифрування.
7. Підготовлено цей звіт.

## в) Код програми

### `src/bbs.h`
```cpp
#include <cmath>
#include<iostream>

class BBS
{
private:
	long p, q, n, x0, decryptionExp;

public:
	BBS();
	~BBS();
	BBS(long p, long q, long seed);

	void setP(long p);
	void setQ(long q);
	void setSeed(long seed);
	int getRandBit();
	long getRandNum();
	void setParams(long p, long q, long seed);
};
```

### `src/bbs.cpp`
```cpp
#include "bbs.h"

//default constructor
BBS::BBS()
{
	//default insecure params
	this->p = 11;
	this->q = 19;
	//seed
	this->x0 = 4;
	//product of P & Q
	this->n = p * q;
}

/*
    set object parameters
    @param blum prime number p
    @param blum prime number q
    @param integer seed
*/
void BBS::setParams(long p, long q, long seed)
{
	//choose p and q such that gcd(p, q) = 1
	this->p = p;
	this->q = q;
	//random seed should be netween 1 & n-1
	this->x0 = seed;
	//blum integer
	this->n = p * q;
}

/*
    overloaded constructor
    @param blum prime number p
    @param blum prime number q
    @param integer seed
*/
BBS::BBS(long p, long q, long seed)
{
	this->setParams(p, q, seed);
}

/*
    Get next random number
*/
long BBS::getRandNum()
{
	long nextRandNum = (this->x0 * this->x0) % this->n;
	this->x0 = nextRandNum;
	return nextRandNum;
}

/*
    Get next random bit
*/
int BBS::getRandBit()
{
	return this->getRandNum() % 2;
}

void BBS::setP(long p)
{
	this->p = p;
}

void BBS::setQ(long q)
{
	this->q = q;
}

void BBS::setSeed(long seed)
{
	this->x0 = seed;
}

BBS::~BBS(){}
```

### `main.cpp`
```cpp
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
    std::string message = "YourLastName"; // Please replace with your actual last name

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
```

## г) Висновок з самостійної роботи

Створена система шифрування на основі генератора BBS є криптографічно стійкою за умови правильного вибору параметрів (великих простих чисел p та q). Перевагою даного методу є його простота реалізації. Однак, швидкість генерації бітів є невисокою, що робить його менш придатним для шифрування великих обсягів даних у реальному часі порівняно з іншими потоковими шифрами (наприклад, AES в режимі CTR).

Якість шифрування напряму залежить від складності факторизації числа n. Для підвищення стійкості слід обирати значно більші прості числа p та q (наприклад, 2048 біт).
