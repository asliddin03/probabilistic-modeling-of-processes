#include "config.h"

#include <QFile>
#include <QJsonObject>

namespace {
    bool isAlmostInteger(double value, double eps = 1e-9) {
        return std::abs(value - std::round(value)) < eps;
    }
}

bool ConfigLoader::loadFromFile(const QString& filePath, SimulationConfig& config, QString& error) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        error = "Не удалось открыть файл конфигурации: " + filePath;
        return false;
    }

    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        error = "Ошибка JSON: " + parseError.errorString();
        return false;
    }

    if (!doc.isObject()) {
        error = "Корень JSON должен быть объектом.";
        return false;
    }

    const QJsonObject obj = doc.object();

    if (obj.contains("M")) config.M = obj["M"].toDouble();
    if (obj.contains("h")) config.h = obj["h"].toDouble();
    if (obj.contains("tau")) config.tau = obj["tau"].toDouble();
    if (obj.contains("m")) config.m = obj["m"].toInt();
    if (obj.contains("N")) config.N = obj["N"].toInt();

    if (obj.contains("distribution") && obj["distribution"].isObject()) {
        const QJsonObject dist = obj["distribution"].toObject();

        if (dist.contains("type"))
            config.distributionType = dist["type"].toString();

        if (dist.contains("p")) {
            config.binomialP = dist["p"].toDouble();
            config.geometricP = dist["p"].toDouble();
        }

        if (dist.contains("binomialP"))
            config.binomialP = dist["binomialP"].toDouble();

        if (dist.contains("geometricP"))
            config.geometricP = dist["geometricP"].toDouble();

        if (dist.contains("mode"))
            config.triangularMode = dist["mode"].toInt();
    }

    return validate(config, error);
}

bool ConfigLoader::validate(const SimulationConfig& config, QString& error) {
    if (config.M <= 0.0) {
        error = "Параметр M должен быть > 0.";
        return false;
    }

    if (config.h <= 0.0) {
        error = "Параметр h должен быть > 0.";
        return false;
    }

    if (config.tau <= 0.0) {
        error = "Параметр tau должен быть > 0.";
        return false;
    }

    if (config.m < 0) {
        error = "Параметр m должен быть >= 0.";
        return false;
    }

    if (config.N <= 0) {
        error = "Параметр N должен быть > 0.";
        return false;
    }

    const double steps = config.M / config.h;
    if (!isAlmostInteger(steps)) {
        error = "Отрезок [0, M] должен делиться на шаг h без остатка.";
        return false;
    }

    if (config.distributionType == "binomial") {
        if (config.binomialP <= 0.0 || config.binomialP >= 1.0) {
            error = "Для биномиального распределения p должно быть в (0, 1).";
            return false;
        }
    }
    else if (config.distributionType == "geometric") {
        if (config.geometricP <= 0.0 || config.geometricP >= 1.0) {
            error = "Для конечного геометрического распределения p должно быть в (0, 1).";
            return false;
        }
    }
    else if (config.distributionType == "triangular") {
        if (config.triangularMode < 0 || config.triangularMode > config.m) {
            error = "Для треугольного распределения mode должен быть в диапазоне [0, m].";
            return false;
        }
    }
    else if (config.distributionType != "uniform") {
        error = "Неизвестный тип распределения: " + config.distributionType;
        return false;
    }

    error.clear();
    return true;
}