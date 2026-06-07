#include "probability.h"
#include "cipher.h"

double probabilityCipher(
    const std::string& cipherType,
    const std::string& alphabet,
    const std::vector<Item>& messages,
    const std::vector<Item>& keys,
    const std::string& targetCipher) {
    double probability = 0.0;

    for (const Item& message : messages) {
        for (const Item& key : keys) {
            std::string currentCipher = encrypt(
                cipherType,
                message.value,
                key.value,
                alphabet
            );

            if (currentCipher == targetCipher) {
                probability += message.probability * key.probability;
            }
        }
    }

    return probability;
}

double probabilityCipherGivenMessage(
    const std::string& cipherType,
    const std::string& alphabet,
    const Item& message,
    const std::vector<Item>& keys,
    const std::string& targetCipher) {
    double probability = 0.0;

    for (const Item& key : keys) {
        std::string currentCipher = encrypt(
            cipherType,
            message.value,
            key.value,
            alphabet
        );

        if (currentCipher == targetCipher) {
            probability += key.probability;
        }
    }

    return probability;
}

std::vector<ResultRow> makeAnalysis(
    const std::string& cipherType,
    const std::string& alphabet,
    const std::vector<Item>& messages,
    const std::vector<Item>& keys,
    const std::string& targetCipher,
    double cipherProbability) {
    std::vector<ResultRow> rows;

    for (const Item& message : messages) {
        ResultRow row;

        row.message = message.value;
        row.messageProbability = message.probability;

        row.cipherGivenMessage = probabilityCipherGivenMessage(
            cipherType,
            alphabet,
            message,
            keys,
            targetCipher
        );

        if (cipherProbability > 0.0) {
            row.messageGivenCipher = message.probability * row.cipherGivenMessage / cipherProbability;
        } else {
            row.messageGivenCipher = 0.0;
        }

        rows.push_back(row);
    }

    return rows;
}