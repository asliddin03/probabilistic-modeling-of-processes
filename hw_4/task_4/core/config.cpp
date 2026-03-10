#include "config.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

static DistributionType parseDistribution(const QString& s) {
    if (s == "uniform")
        return DistributionType::Uniform;
    if (s == "binomial")
        return DistributionType::Binomial;
    if (s == "geometric")
        return DistributionType::Geometric;
    if (s == "triangular")
        return DistributionType::Triangular;

    return DistributionType::Uniform;
}

bool ConfigLoader::loadFromJson(const QString& filePath, AppConfig& config, QString& error) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        error = "Не удалось открыть config.json";
        return false;
    }

    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        error = "Ошибка парсинга JSON: " + parseError.errorString();
        return false;
    }

    if (!doc.isObject())
    {
        error = "JSON должен содержать объект";
        return false;
    }

    const QJsonObject obj = doc.object();

    config.rho = obj.value("rho").toDouble(20.0);
    config.n = obj.value("n").toInt(8);
    config.M = obj.value("M").toInt(100);
    config.K = obj.value("K").toInt(50);

    config.distribution = parseDistribution(obj.value("distribution").toString("uniform"));

    config.binomialP = obj.value("binomial_p").toDouble(0.5);
    config.geometricQ = obj.value("geometric_q").toDouble(0.35);
    config.triangularMode = obj.value("triangular_mode").toInt(config.n / 2);

    if (config.rho <= 0.0)
    {
        error = "rho должно быть > 0";
        return false;
    }

    if (config.n < 4)
    {
        error = "n должно быть >= 4";
        return false;
    }

    if (config.M <= 0)
    {
        error = "M должно быть > 0";
        return false;
    }

    if (config.K <= 0)
    {
        error = "K должно быть > 0";
        return false;
    }

    if (config.binomialP < 0.0 || config.binomialP > 1.0)
    {
        error = "binomial_p должно быть в [0, 1]";
        return false;
    }

    if (config.geometricQ <= 0.0 || config.geometricQ >= 1.0)
    {
        error = "geometric_q должно быть в (0, 1)";
        return false;
    }

    if (config.triangularMode < 0 || config.triangularMode >= config.n)
    {
        error = "triangular_mode должно быть в диапазоне [0, n-1]";
        return false;
    }

    return true;
}