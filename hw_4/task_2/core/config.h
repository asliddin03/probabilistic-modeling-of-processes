#pragma once

#include <QString>
#include <QVector>

enum class DistributionType {
    Uniform,
    Binomial,
    FiniteGeometric,
    Triangular,
};

struct Config {
    double Y = 0.0;
    double h = 1.0;
    int steps = 10;

    QVector<double> sValues;

    DistributionType distributionType = DistributionType::Uniform;

    double p = 0.5;
    int trials = 1;
    int shift = 0;

    int K = 10;
    int l = 0;
    int experiments = 10000;

    static Config fromJsonFile(const QString& filePath);
};