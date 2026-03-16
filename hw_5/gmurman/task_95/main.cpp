#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

class UrnSimulation {
private:
    std::mt19937 gen;

public:
    UrnSimulation() : gen(std::random_device{}()) {}

    // true = белый, false = черный
    bool randomBallFromUrn(const std::vector<bool>& urn) {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(urn.size()) - 1);
        return urn[dist(gen)];
    }

    bool oneExperiment() {
        // false = черный, true = белый
        std::vector<bool> urn1 = {
            false, false, false, false, false, false,
            true, true, true, true
        };

        std::vector<bool> urn2 = {
            false, false, false, false, false, false,
            true, true, true, true
        };

        std::vector<bool> urn3 = {
            false, false, false, false, false, false,
            true, true, true, true
        };

        // Из 1-й во 2-ю
        bool movedToSecond = randomBallFromUrn(urn1);
        urn2.push_back(movedToSecond);

        // Из 2-й в 3-ю
        bool movedToThird = randomBallFromUrn(urn2);
        urn3.push_back(movedToThird);

        // Случайный шар из 3-й урны
        bool selectedFromThird = randomBallFromUrn(urn3);

        return selectedFromThird; // true, если белый
    }

    double run(int experiments, int& whiteCount) {
        whiteCount = 0;

        for (int i = 0; i < experiments; ++i) {
            if (oneExperiment()) {
                ++whiteCount;
            }
        }

        return static_cast<double>(whiteCount) / experiments;
    }
};

int main() {
    int n;
    std::cout << "Введите число экспериментов: ";
    std::cin >> n;

    if (n <= 0) {
        std::cout << "Число экспериментов должно быть положительным.\n";
        return 1;
    }

    UrnSimulation simulation;

    int whiteCount = 0;
    double empirical = simulation.run(n, whiteCount);
    double theoretical = 0.4;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nРезультаты моделирования:\n";
    std::cout << "Количество экспериментов: " << n << "\n";
    std::cout << "Белый шар из 3-й урны:    " << whiteCount << "\n";
    std::cout << "Эмпирическая вероятность: " << empirical << "\n";
    std::cout << "Теоретическая вероятность:" << theoretical << "\n";

    return 0;
}