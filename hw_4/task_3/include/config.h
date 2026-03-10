#pragma once

#include <QString>

struct AppConfig
{
    double p = 0.5;
    unsigned long long N = 10;
    long long sPlus = 1;
    long long sMinus = 1;
    int experiments = 10000;
};

class ConfigLoader {
public:
    static bool loadFromFile(const QString& filePath, AppConfig& config, QString& error);
};