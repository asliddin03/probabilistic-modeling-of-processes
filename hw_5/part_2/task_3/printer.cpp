#include "printer.h"
#include "cipher.h"

#include <iostream>
#include <iomanip>

void printEncryptionTable(
    const std::string& cipherType,
    const std::string& alphabet,
    const std::vector<Item>& messages,
    const std::vector<Item>& keys) {
    std::cout << "\nТаблица шифрования:\n\n";

    std::cout << std::left
              << std::setw(15) << "M"
              << std::setw(15) << "Key"
              << std::setw(15) << "Cipher"
              << std::setw(15) << "P(M)"
              << std::setw(15) << "P(Key)"
              << '\n';

    std::cout << std::string(75, '-') << '\n';

    for (const Item& message : messages) {
        for (const Item& key : keys) {
            std::string cipher = encrypt(
                cipherType,
                message.value,
                key.value,
                alphabet
            );

            std::cout << std::left
                      << std::setw(15) << message.value
                      << std::setw(15) << key.value
                      << std::setw(15) << cipher
                      << std::setw(15) << message.probability
                      << std::setw(15) << key.probability
                      << '\n';
        }
    }
}

void printAnalysis(
    const std::vector<ResultRow>& rows,
    double cipherProbability,
    const std::string& targetCipher) {
    std::cout << "\nАнализ для шифртекста: " << targetCipher << "\n\n";

    std::cout << "P(Cipher = " << targetCipher << ") = "
              << cipherProbability << "\n\n";

    std::cout << std::left
              << std::setw(20) << "M"
              << std::setw(20) << "P(M)"
              << std::setw(30) << "P(Cipher | M)"
              << std::setw(30) << "P(M | Cipher)"
              << '\n';

    std::cout << std::string(100, '-') << '\n';

    for (const ResultRow& row : rows) {
        std::cout << std::left
                  << std::setw(20) << row.message
                  << std::setw(20) << row.messageProbability
                  << std::setw(30) << row.cipherGivenMessage
                  << std::setw(30) << row.messageGivenCipher
                  << '\n';
    }
}

void printBestAttackVariant(const std::vector<ResultRow>& rows) {
    if (rows.empty()) {
        return;
    }

    ResultRow best = rows[0];

    for (const ResultRow& row : rows) {
        if (row.messageGivenCipher > best.messageGivenCipher) {
            best = row;
        }
    }

    std::cout << "\nНаиболее вероятный открытый текст:\n";
    std::cout << "M = " << best.message << '\n';
    std::cout << "P(M | Cipher) = " << best.messageGivenCipher << '\n';
}