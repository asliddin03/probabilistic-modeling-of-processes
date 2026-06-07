#pragma once
#include <string>

struct Item {
    std::string value;
    double probability;
};

struct ResultRow {
    std::string message;
    double messageProbability;
    double cipherGivenMessage;
    double messageGivenCipher;
};