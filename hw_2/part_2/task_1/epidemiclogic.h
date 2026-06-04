#pragma once

#include <vector>
#include <string>

class EpidemicLogic {
public:
    enum State {
        Healthy = 0,
        Infected = 1,
        Recovered = 2
    };

    EpidemicLogic();

    bool loadFromFile(const std::string& fileName, std::string& errorMessage);

    void setProbabilities(double infectionProbability, double recoveryProbability);
    void simulate(int steps);

    std::string getAllPeopleInfo() const;

    std::vector<int> findNotInfectedPeople() const;
    std::vector<int> findRecoveredPeople() const;
    std::vector<int> findPeopleWithNotRecoveredEnvironment() const;
    std::vector<int> findInfectedWithAllInfectedEnvironment() const;

    std::string vectorToString(const std::vector<int>& people) const;

    int peopleCount() const;
    int edgesCount() const;

private:
    int n;
    int m;

    std::vector<std::vector<int>> graph;
    std::vector<State> states;
    std::vector<bool> wasInfected;

    double p1;
    double p2;

    bool chance(double probability) const;
    std::string stateToString(State state) const;
};