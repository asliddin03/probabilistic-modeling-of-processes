#pragma once

#include <string>
#include <vector>
#include "models.h"

bool readInputFile(
    const std::string& filename,
    std::string& cipherType,
    std::string& alphabet,
    std::vector<Item>& messages,
    std::vector<Item>& keys,
    std::string& targetCipher
);