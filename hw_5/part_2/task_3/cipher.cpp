#include "cipher.h"

int findInAlphabet(char ch, const std::string& alphabet) {
    for (int i = 0; i < static_cast<int>(alphabet.size()); ++i) {
        if (alphabet[i] == ch) {
            return i;
        }
    }

    return -1;
}

std::string encryptVigenere(const std::string& message, const std::string& key,
                                                const std::string& alphabet ) {
    std::string cipher;
    int alphabetSize = static_cast<int>(alphabet.size());

    for (int i = 0; i < static_cast<int>(message.size()); ++i) {
        int mIndex = findInAlphabet(message[i], alphabet);
        int kIndex = findInAlphabet(key[i % key.size()], alphabet);

        if (mIndex == -1 || kIndex == -1) {
            return "";
        }

        int cIndex = (mIndex + kIndex) % alphabetSize;
        cipher += alphabet[cIndex];
    }

    return cipher;
}

std::string encryptVernam(const std::string& message, const std::string& key) {
    if (message.size() != key.size()) {
        return "";
    }

    std::string cipher;

    for (int i = 0; i < static_cast<int>(message.size()); ++i) {
        if ((message[i] != '0' && message[i] != '1') || (key[i] != '0' && key[i] != '1')) {
            return "";
        }

        if (message[i] == key[i]) {
            cipher += '0';
        } else {
            cipher += '1';
        }
    }

    return cipher;
}

std::string encrypt(
    const std::string& cipherType,
    const std::string& message,
    const std::string& key,
    const std::string& alphabet) {
    if (cipherType == "VIGENERE") {
        return encryptVigenere(message, key, alphabet);
    }

    if (cipherType == "VERNAM") {
        return encryptVernam(message, key);
    }

    return "";
}