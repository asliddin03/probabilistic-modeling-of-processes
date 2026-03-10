#pragma once

#include <QString>

enum class DistributionType {
    Uniform,
    Binomial,
    Geometric,
    Triangular
};

struct AppConfig {
    double rho = 20.0;
    int n = 8;
    int M = 100;
    int K = 50;

    DistributionType distribution = DistributionType::Uniform;

    double binomialP = 0.5;
    double geometricQ = 0.35;
    int triangularMode = 0;
};

class ConfigLoader {
public:
    static bool loadFromJson(const QString& filePath, AppConfig& config, QString& error);
};