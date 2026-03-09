#include "logger.h"

SimulationLogger::SimulationLogger(int maxEntries)
    : m_maxEntries(maxEntries) {}

void SimulationLogger::addEntry(const LogEntry& entry) {
    m_entries.push_back(entry);

    while (m_entries.size() > m_maxEntries)
        m_entries.removeFirst();
}

const QVector<LogEntry>& SimulationLogger::entries() const {
    return m_entries;
}