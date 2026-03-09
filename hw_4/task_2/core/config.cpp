#include "config.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QException>
#include <stdexcept>

static DistributionType parseDistributionType(const QString& type) {
    if (type == "uniform")
        return DistributionType::Uniform;
    if (type == "binomial")
        return DistributionType::Binomial;
    if (type == "finite_geometric")
        return DistributionType::FiniteGeometric;
    if (type == "triangular")
        return DistributionType::Triangular;

    throw std::runtime_error("Unknown distribution type");
}

Config Config::fromJsonFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Cannot open config file");

    const QByteArray data = file.readAll();
    file.close();

    const QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject())
        throw std::runtime_error("Invalid JSON format");

    const QJsonObject root = doc.object();

    Config cfg;

    cfg.Y = root.value("Y").toDouble();
    cfg.h = root.value("h").toDouble();
    cfg.steps = root.value("steps").toInt();
    cfg.K = root.value("K").toInt();
    cfg.l = root.value("l").toInt();
    cfg.experiments = root.value("experiments").toInt();

    const QJsonArray array = root.value("s_values").toArray();
    for (const QJsonValue& value : array) {
        cfg.sValues.append(value.toDouble());
    }

    const QJsonObject distObj = root.value("distribution").toObject();
    cfg.distributionType = parseDistributionType(distObj.value("type").toString());
    cfg.p = distObj.value("p").toDouble(0.5);
    cfg.trials = root.value("trials").toInt(1);
    cfg.shift = root.value("shift").toInt(0);

    if (cfg.h <= 0.0)
        throw std::runtime_error("h must be > 0");
    if (cfg.steps <= 0)
        throw std::runtime_error("steps must be > 0");
    if (cfg.K <= 0)
        throw std::runtime_error("K must be > 0");
    if (cfg.experiments <= 0)
        throw std::runtime_error("experiments must be > 0");
    if (cfg.l < 0)
        throw std::runtime_error("l must be >= 0");

    if ((cfg.distributionType == DistributionType::Uniform ||
         cfg.distributionType == DistributionType::FiniteGeometric ||
         cfg.distributionType == DistributionType::Triangular) &&
        cfg.sValues.isEmpty())
    {
        throw std::runtime_error("s_values must not be empty");
    }

    if ((cfg.distributionType == DistributionType::Binomial ||
         cfg.distributionType == DistributionType::FiniteGeometric) &&
        (cfg.p < 0.0 || cfg.p > 1.0))
    {
        throw std::runtime_error("p must be in [0, 1]");
    }

    if (cfg.distributionType == DistributionType::Binomial && cfg.trials <= 0)
        throw std::runtime_error("trials must be > 0");

    return cfg;
}
