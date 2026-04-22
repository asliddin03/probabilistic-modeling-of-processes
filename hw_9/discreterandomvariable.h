#pragma once

#include <QMap>
#include <QVector>
#include <QPair>
#include <QDataStream>
#include <QString>

class DiscreteRandomVariable {
public:
    DiscreteRandomVariable() = default;
    explicit DiscreteRandomVariable(const QVector<QPair<double, double>>& table);

    void setTable(const QVector<QPair<double, double>>& table);
    QVector<QPair<double, double>> table() const;

    bool isValid(double eps = 1e-9) const;
    QString validationError(double eps = 1e-9) const;

    DiscreteRandomVariable operator*(double scalar) const;
    DiscreteRandomVariable operator+(const DiscreteRandomVariable& other) const;
    DiscreteRandomVariable operator*(const DiscreteRandomVariable& other) const;

    double expectedValue() const;
    double variance() const;
    double skewness() const;
    double kurtosis() const;

    QVector<QPair<double, double>> distributionLaw() const;
    QVector<QPair<double, double>> polylinePoints() const;
    QVector<QPair<double, double>> cdfPoints() const;

    friend QDataStream& operator<<(QDataStream& out, const DiscreteRandomVariable& rv);
    friend QDataStream& operator>>(QDataStream& in, DiscreteRandomVariable& rv);

private:
    QMap<double, double> m_distribution;
    static constexpr double EPS = 1e-9;
};