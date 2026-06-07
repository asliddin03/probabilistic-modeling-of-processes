#pragma once

#include <string>

std::string encrypt(
    const std::string& cipherType,
    const std::string& message,
    const std::string& key,
    const std::string& alphabet
);

std::string encryptVigenere(
    const std::string& message,
    const std::string& key,
    const std::string& alphabet
);

std::string encryptVernam(
    const std::string& message,
    const std::string& key
);