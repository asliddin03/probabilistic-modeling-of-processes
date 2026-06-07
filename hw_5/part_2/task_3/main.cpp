#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "models.h"
#include "file_reader.h"
#include "probability.h"
#include "printer.h"

int main() {
    std::string filename;

    std::cout << "Введите имя входного файла: ";
    std::cin >> filename;

    std::string cipherType;
    std::string alphabet;
    std::vector<Item> messages;
    std::vector<Item> keys;
    std::string targetCipher;

    bool ok = readInputFile(
        filename,
        cipherType,
        alphabet,
        messages,
        keys,
        targetCipher
    );

    if (!ok) {
        return 1;
    }

    double cipherProbability = probabilityCipher(
        cipherType,
        alphabet,
        messages,
        keys,
        targetCipher
    );

    std::vector<ResultRow> rows = makeAnalysis(
        cipherType,
        alphabet,
        messages,
        keys,
        targetCipher,
        cipherProbability
    );

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nТип шифра: " << cipherType << '\n';
    std::cout << "Алфавит: " << alphabet << '\n';

    printEncryptionTable(cipherType, alphabet, messages, keys);
    printAnalysis(rows, cipherProbability, targetCipher);
    printBestAttackVariant(rows);

    return 0;
}