#include "discreterandomvariable.h"

#include <QtMath>
#include <stdexcept>

DiscreteRandomVariable::DiscreteRandomVariable(const QVector<QPair<double, double>>& table)
{
    setTable(table);
}

void DiscreteRandomVariable::setTable(const QVector<QPair<double, double>>& table)
{
    m_distribution.clear();

    for (const auto& pair : table) {
        double x = pair.first;
        double p = pair.second;

        if (m_distribution.contains(x)) {
            throw std::invalid_argument("Значения случайной величины должны быть попарно различны.");
        }

        m_distribution[x] = p;
    }

    if (!isValid()) {
        throw std::invalid_argument(validationError().toStdString());
    }
}

QVector<QPair<double, double>> DiscreteRandomVariable::table() const
{
    QVector<QPair<double, double>> result;
    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        result.append(qMakePair(it.key(), it.value()));
    }
    return result;
}

bool DiscreteRandomVariable::isValid(double eps) const
{
    if (m_distribution.isEmpty()) {
        return false;
    }

    double sum = 0.0;
    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        if (it.value() < -eps) {
            return false;
        }
        sum += it.value();
    }

    return qAbs(sum - 1.0) <= eps;
}

QString DiscreteRandomVariable::validationError(double eps) const
{
    if (m_distribution.isEmpty()) {
        return "Распределение пусто.";
    }

    double sum = 0.0;
    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        if (it.value() < -eps) {
            return "Вероятности должны быть неотрицательными.";
        }
        sum += it.value();
    }

    if (qAbs(sum - 1.0) > eps) {
        return QString("Сумма вероятностей должна быть равна 1. Сейчас: %1")
            .arg(sum, 0, 'g', 12);
    }

    return {};
}

DiscreteRandomVariable DiscreteRandomVariable::operator*(double scalar) const
{
    QMap<double, double> merged;

    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        double newValue = it.key() * scalar;
        merged[newValue] += it.value();
    }

    QVector<QPair<double, double>> result;
    for (auto it = merged.begin(); it != merged.end(); ++it) {
        result.append(qMakePair(it.key(), it.value()));
    }

    return DiscreteRandomVariable(result);
}

DiscreteRandomVariable DiscreteRandomVariable::operator+(const DiscreteRandomVariable& other) const
{
    QMap<double, double> resultMap;

    for (auto it1 = m_distribution.begin(); it1 != m_distribution.end(); ++it1) {
        for (auto it2 = other.m_distribution.begin(); it2 != other.m_distribution.end(); ++it2) {
            double value = it1.key() + it2.key();
            double probability = it1.value() * it2.value();
            resultMap[value] += probability;
        }
    }

    QVector<QPair<double, double>> result;
    for (auto it = resultMap.begin(); it != resultMap.end(); ++it) {
        result.append(qMakePair(it.key(), it.value()));
    }

    return DiscreteRandomVariable(result);
}

DiscreteRandomVariable DiscreteRandomVariable::operator*(const DiscreteRandomVariable& other) const
{
    QMap<double, double> resultMap;

    for (auto it1 = m_distribution.begin(); it1 != m_distribution.end(); ++it1) {
        for (auto it2 = other.m_distribution.begin(); it2 != other.m_distribution.end(); ++it2) {
            double value = it1.key() * it2.key();
            double probability = it1.value() * it2.value();
            resultMap[value] += probability;
        }
    }

    QVector<QPair<double, double>> result;
    for (auto it = resultMap.begin(); it != resultMap.end(); ++it) {
        result.append(qMakePair(it.key(), it.value()));
    }

    return DiscreteRandomVariable(result);
}

double DiscreteRandomVariable::expectedValue() const
{
    double result = 0.0;
    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        result += it.key() * it.value();
    }
    return result;
}

double DiscreteRandomVariable::variance() const
{
    double ex = expectedValue();
    double ex2 = 0.0;

    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        ex2 += it.key() * it.key() * it.value();
    }

    return ex2 - ex * ex;
}

double DiscreteRandomVariable::skewness() const
{
    double mu = expectedValue();
    double var = variance();

    if (qAbs(var) < EPS) {
        return 0.0;
    }

    double sigma = qSqrt(var);
    double mu3 = 0.0;

    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        mu3 += qPow(it.key() - mu, 3) * it.value();
    }

    return mu3 / qPow(sigma, 3);
}

double DiscreteRandomVariable::kurtosis() const
{
    double mu = expectedValue();
    double var = variance();

    if (qAbs(var) < EPS) {
        return 0.0;
    }

    double mu4 = 0.0;
    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        mu4 += qPow(it.key() - mu, 4) * it.value();
    }

    return mu4 / qPow(var, 2) - 3.0;
}

QVector<QPair<double, double>> DiscreteRandomVariable::distributionLaw() const
{
    return table();
}

QVector<QPair<double, double>> DiscreteRandomVariable::polylinePoints() const
{
    return table();
}

QVector<QPair<double, double>> DiscreteRandomVariable::cdfPoints() const
{
    QVector<QPair<double, double>> result;
    double cumulative = 0.0;

    for (auto it = m_distribution.begin(); it != m_distribution.end(); ++it) {
        cumulative += it.value();
        result.append(qMakePair(it.key(), cumulative));
    }

    return result;
}

QDataStream& operator<<(QDataStream& out, const DiscreteRandomVariable& rv)
{
    out << static_cast<quint32>(rv.m_distribution.size());

    for (auto it = rv.m_distribution.begin(); it != rv.m_distribution.end(); ++it) {
        out << it.key() << it.value();
    }

    return out;
}

QDataStream& operator>>(QDataStream& in, DiscreteRandomVariable& rv)
{
    rv.m_distribution.clear();

    quint32 size = 0;
    in >> size;

    for (quint32 i = 0; i < size; ++i) {
        double x = 0.0;
        double p = 0.0;
        in >> x >> p;
        rv.m_distribution[x] = p;
    }

    if (!rv.isValid()) {
        rv.m_distribution.clear();
        throw std::invalid_argument("Некорректные данные при десериализации.");
    }

    return in;
}