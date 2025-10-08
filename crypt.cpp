#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <filesystem>
#include <unordered_map>


const std::string ENG_ALPHABET = "abcdefghijklmnopqrstuvwxyz";

std::unordered_map<char, char> createSubstitutionMap(const std::string& passWord, int indent)
{
    std::cout << "Створюємо словник заміни символів...\n";
    std::string cleanPassword = "";
    for (char c : passWord)
    {
        char lowerC = std::tolower(static_cast<unsigned char>(c));
        if (
            cleanPassword.find(lowerC) == std::string::npos &&
            ENG_ALPHABET.find(lowerC) != std::string::npos
        )
        {
            cleanPassword += lowerC;
        }
    }
    
    std::string newAlphabet = "";
    
    for (int i = 0; i < indent && i < 26; ++i)
    {
        if (cleanPassword.find(ENG_ALPHABET[i]) == std::string::npos)
        {
            newAlphabet += ENG_ALPHABET[i];
        }
    }
    
    newAlphabet += cleanPassword;
    
    for (char c : ENG_ALPHABET)
    {
        if (newAlphabet.find(c) == std::string::npos)
        {
            newAlphabet += c;
        }
    }
    
    std::cout << "Будуємо карту заміни для швидкого пошуку...\n";
    std::unordered_map<char, char> substitutionMap;
    for (size_t i = 0; i < ENG_ALPHABET.size(); ++i)
    {
        substitutionMap[ENG_ALPHABET[i]] = newAlphabet[i];
    }
    std::cout << "Словник заміни готовий!\n";
    
    return substitutionMap;
}

char substituteChar(
    char ch,
    const std::unordered_map<char, char>& substitutionMap)
{
    char lower_ch = std::tolower(static_cast<unsigned char>(ch));
    
    auto it = substitutionMap.find(lower_ch);
    if (it == substitutionMap.end())
    {
        return ch;
    }
    
    char new_char = it->second;
    
    if (std::isupper(static_cast<unsigned char>(ch)))
    {
        return std::toupper(static_cast<unsigned char>(new_char));
    }
    else
    {
        return new_char;
    }
}

void processAndReplaceFile(
    const std::string& fileName,
    const std::unordered_map<char, char>& substitutionMap)
{
    std::cout << "Починаємо обробку файлу: " << fileName << "\n";
    std::string tempFileName = fileName + ".temp";

    std::cout << "Відкриваємо файл для читання...\n";
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open())
    {
        std::cerr << "Помилка: Не вдалося відкрити файл для читання.\n";
        return;
    }
    std::cout << "Файл успішно відкрито!\n";

    std::cout << "Створюємо тимчасовий файл...\n";
    std::ofstream tempFile(tempFileName);
    if (!tempFile.is_open())
    {
        std::cerr << "Помилка: Не вдалося створити тимчасовий файл.\n";
        inputFile.close();
        return;
    }
    std::cout << "Тимчасовий файл створено!\n";

    std::cout << "Починаємо шифрування тексту...\n";
    char ch;
    while (inputFile.get(ch))
    {
        if (
            std::isspace(static_cast<unsigned char>(ch)) || 
            std::ispunct(static_cast<unsigned char>(ch))
            )
        {
            tempFile.put(ch);
        }
        else
        {
            tempFile.put(substituteChar(ch, substitutionMap));
        }
    }

    inputFile.close();
    tempFile.close();
    std::cout << "Шифрування завершено! Замінюємо оригінальний файл...\n";

    try
    {
        std::filesystem::remove(fileName);
        std::filesystem::rename(tempFileName, fileName);
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Помилка файлової системи: " << e.what() << '\n';
        if (std::filesystem::exists(tempFileName))
        {
            std::filesystem::remove(tempFileName);
        }
        return;
    }

    std::cout << "Файл " << fileName << " успішно зашифровано та оновлено!\n";
}

int main()
{
    std::string filePath = "my_file.txt";

    std::string passWord;
    int indent;

    std::cout << "Ініціалізація програми шифрування...\n";
    std::cout << "Пароль: ";
    std::cin >> passWord;
    std::cout << "Позиція: ";
    std::cin >> indent;
    std::cout << "\n";
    
    std::unordered_map<char, char> substitutionMap = createSubstitutionMap(passWord, indent);
    
    std::cout << "Оригінальний алфавіт: " << ENG_ALPHABET << "\n";
    std::cout << "Алфавіт заміни: ";
    for (char c : ENG_ALPHABET)
    {
        std::cout << substitutionMap[c];
    }
    std::cout << "\n";

    std::cout << "Перевіряємо існування файлу...\n";
    if (std::filesystem::exists(filePath))
    {
        std::cout << "Файл знайдено! Починаємо шифрування...\n";
        processAndReplaceFile(filePath, substitutionMap);
        std::cout << "Операція завершена успішно!\n";
    }
    else
    {
        std::cerr << "Помилка: Файл " << filePath << " не існує.\n";
        return 1;
    }

    return 0;
}