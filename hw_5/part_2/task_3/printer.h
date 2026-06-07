#pragma once

#include <string>
#include <vector>
#include "models.h"

void printEncryptionTable(
    const std::string& cipherType,
    const std::string& alphabet,
    const std::vector<Item>& messages,
    const std::vector<Item>& keys
);

void printAnalysis(
    const std::vector<ResultRow>& rows,
    double cipherProbability,
    const std::string& targetCipher
);

void printBestAttackVariant(const std::vector<ResultRow>& rows);