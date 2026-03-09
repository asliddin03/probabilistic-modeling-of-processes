#pragma once

#include <QString>
#include <QDateTime>
#include <QVector>

struct LogEntry
{
    QDateTime timestamp;
    QString distributionName;
    int crossings = 0;
    double estimatedProbability = 0.0;
    QString summary;
};

class SimulationLogger {
    public:
    explicit SimulationLogger(int maxEntries);

    void addEntry(const LogEntry& entry);
    const QVector<LogEntry>& entries() const;

private:
    int m_maxEntries = 10;
    QVector<LogEntry> m_entries;
};