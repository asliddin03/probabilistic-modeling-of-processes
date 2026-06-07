#pragma once

#include <string>
#include <vector>
#include "models.h"

double probabilityCipher(
    const std::string& cipherType,
    const std::string& alphabet,
    const std::vector<Item>& messages,
    const std::vector<Item>& keys,
    const std::string& targetCipher
);

double probabilityCipherGivenMessage(
    const std::string& cipherType,
    const std::string& alphabet,
    const Item& message,
    const std::vector<Item>& keys,
    const std::string& targetCipher
);

std::vector<ResultRow> makeAnalysis(
    const std::string& cipherType,
    const std::string& alphabet,
    const std::vector<Item>& messages,
    const std::vector<Item>& keys,
    const std::string& targetCipher,
    double cipherProbability
);