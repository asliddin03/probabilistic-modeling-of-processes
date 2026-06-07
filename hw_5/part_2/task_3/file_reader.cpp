#include "file_reader.h"

#include <fstream>
#include <iostream>

double sumProbabilities(const std::vector<Item>& items) {
    double sum = 0.0;

    for (const Item& item : items) {
        sum += item.probability;
    }

    return sum;
}

void normalizeProbabilities(std::vector<Item>& items) {
    double sum = sumProbabilities(items);

    if (sum == 0.0) {
        return;
    }

    for (Item& item : items) {
        item.probability /= sum;
    }
}

bool readInputFile(
    const std::string& filename,
    std::string& cipherType,
    std::string& alphabet,
    std::vector<Item>& messages,
    std::vector<Item>& keys,
    std::string& targetCipher
)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл.\n";
        return false;
    }

    file >> cipherType;
    file >> alphabet;

    int messageCount;
    file >> messageCount;

    if (messageCount <= 0) {
        std::cout << "Ошибка: некорректное количество открытых текстов.\n";
        return false;
    }

    messages.clear();

    for (int i = 0; i < messageCount; ++i) {
        Item item;
        file >> item.value >> item.probability;

        if (item.probability < 0.0) {
            std::cout << "Ошибка: вероятность открытого текста не может быть отрицательной.\n";
            return false;
        }

        messages.push_back(item);
    }

    int keyCount;
    file >> keyCount;

    if (keyCount <= 0) {
        std::cout << "Ошибка: некорректное количество ключей.\n";
        return false;
    }

    keys.clear();

    for (int i = 0; i < keyCount; ++i) {
        Item item;
        file >> item.value >> item.probability;

        if (item.probability < 0.0) {
            std::cout << "Ошибка: вероятность ключа не может быть отрицательной.\n";
            return false;
        }

        keys.push_back(item);
    }

    file >> targetCipher;

    if (cipherType != "VIGENERE" && cipherType != "VERNAM") {
        std::cout << "Ошибка: неизвестный тип шифра.\n";
        return false;
    }

    if (targetCipher.empty()) {
        std::cout << "Ошибка: не задан шифртекст.\n";
        return false;
    }

    normalizeProbabilities(messages);
    normalizeProbabilities(keys);

    return true;
}