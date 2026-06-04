#include "epidemiclogic.h"

#include <fstream>
#include <sstream>
#include <random>

EpidemicLogic::EpidemicLogic() {
    n = 0;
    m = 0;

    p1 = 0.3;
    p2 = 0.2;
}

bool EpidemicLogic::loadFromFile(const std::string& fileName, std::string& errorMessage) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        errorMessage = "Не удалось открыть файл.";
        return false;
    }

    if (!(file >> n >> m)) {
        errorMessage = "Ошибка чтения количества людей и знакомств.";
        return false;
    }

    if (n <= 0 || m < 0) {
        errorMessage = "Некорректное количество людей или знакомств.";
        return false;
    }

    graph.assign(n + 1, std::vector<int>());
    states.assign(n + 1, Healthy);
    wasInfected.assign(n + 1, false);

    for (int i = 0; i < m; ++i) {
        int a, b;

        if (!(file >> a >> b)) {
            errorMessage = "Ошибка чтения списка знакомств.";
            return false;
        }

        if (a < 1 || a > n || b < 1 || b > n || a == b) {
            errorMessage = "Некорректная связь между людьми.";
            return false;
        }

        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    int infectedCount;

    if (!(file >> infectedCount)) {
        errorMessage = "Ошибка чтения количества начально заражённых людей.";
        return false;
    }

    if (infectedCount < 0 || infectedCount > n) {
        errorMessage = "Некорректное количество начально заражённых людей.";
        return false;
    }

    for (int i = 0; i < infectedCount; ++i) {
        int person;

        if (!(file >> person)) {
            errorMessage = "Ошибка чтения начально заражённых людей.";
            return false;
        }

        if (person < 1 || person > n) {
            errorMessage = "Некорректный номер начально заражённого человека.";
            return false;
        }

        states[person] = Infected;
        wasInfected[person] = true;
    }

    errorMessage.clear();
    return true;
}

void EpidemicLogic::setProbabilities(double infectionProbability, double recoveryProbability) {
    p1 = infectionProbability;
    p2 = recoveryProbability;
}

void EpidemicLogic::simulate(int steps) {
    if (n == 0 || steps <= 0)
        return;

    for (int step = 0; step < steps; ++step) {
        std::vector<State> newStates = states;

        for (int person = 1; person <= n; ++person) {
            if (states[person] == Infected) {
                for (int neighbor : graph[person]) {
                    if (states[neighbor] == Healthy && chance(p1)) {
                        newStates[neighbor] = Infected;
                        wasInfected[neighbor] = true;
                    }
                }

                if (chance(p2)) {
                    newStates[person] = Recovered;
                }
            }
        }

        states = newStates;
    }
}

std::string EpidemicLogic::getAllPeopleInfo() const {
    if (n == 0)
        return "Данные не загружены.";

    std::ostringstream out;

    out << "Количество людей: " << n << "\n";
    out << "Количество знакомств: " << m << "\n\n";

    for (int person = 1; person <= n; ++person) {
        out << "Человек " << person << ": ";
        out << stateToString(states[person]);
        out << ". Окружение: ";

        if (graph[person].empty()) {
            out << "нет";
        }
        else {
            for (size_t i = 0; i < graph[person].size(); ++i) {
                out << graph[person][i];

                if (i + 1 < graph[person].size())
                    out << ", ";
            }
        }

        out << "\n";
    }

    return out.str();
}

std::vector<int> EpidemicLogic::findNotInfectedPeople() const {
    std::vector<int> result;

    for (int person = 1; person <= n; ++person) {
        if (!wasInfected[person])
            result.push_back(person);
    }

    return result;
}

std::vector<int> EpidemicLogic::findRecoveredPeople() const {
    std::vector<int> result;

    for (int person = 1; person <= n; ++person) {
        if (states[person] == Recovered)
            result.push_back(person);
    }

    return result;
}

std::vector<int> EpidemicLogic::findPeopleWithNotRecoveredEnvironment() const {
    std::vector<int> result;

    for (int person = 1; person <= n; ++person) {
        bool hasNotRecoveredNeighbor = false;

        for (int neighbor : graph[person]) {
            if (states[neighbor] != Recovered) {
                hasNotRecoveredNeighbor = true;
                break;
            }
        }

        if (hasNotRecoveredNeighbor)
            result.push_back(person);
    }

    return result;
}

std::vector<int> EpidemicLogic::findInfectedWithAllInfectedEnvironment() const {
    std::vector<int> result;

    for (int person = 1; person <= n; ++person) {
        if (states[person] != Infected)
            continue;

        if (graph[person].empty())
            continue;

        bool allNeighborsInfected = true;

        for (int neighbor : graph[person]) {
            if (states[neighbor] != Infected) {
                allNeighborsInfected = false;
                break;
            }
        }

        if (allNeighborsInfected)
            result.push_back(person);
    }

    return result;
}

std::string EpidemicLogic::vectorToString(const std::vector<int>& people) const {
    if (people.empty())
        return "Подходящих людей не найдено.";

    std::ostringstream out;

    for (size_t i = 0; i < people.size(); ++i) {
        out << people[i];

        if (i + 1 < people.size())
            out << ", ";
    }

    return out.str();
}

int EpidemicLogic::peopleCount() const {
    return n;
}

int EpidemicLogic::edgesCount() const {
    return m;
}

bool EpidemicLogic::chance(double probability) const {
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator) < probability;
}

std::string EpidemicLogic::stateToString(State state) const {
    if (state == Healthy)
        return "здоров";

    if (state == Infected)
        return "заражён";

    return "исцелился";
}