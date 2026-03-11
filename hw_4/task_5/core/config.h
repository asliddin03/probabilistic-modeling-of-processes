#pragma once

#include "types.h"

class ConfigLoader {
    public:
    static bool loadFromFile(const QString& filePath, SimulationConfig& config, QString& error);
    static bool validate(const SimulationConfig& config, QString& error);
};