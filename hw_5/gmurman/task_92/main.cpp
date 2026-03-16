#include <iostream>
#include <random>
#include <iomanip>

class ShootingSimulation {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> rifleDist;   // выбор винтовки: 0..4
    std::uniform_real_distribution<double> probDist; // случайное число [0,1)

public:
    ShootingSimulation()
        : rng(std::random_device{}()),
          rifleDist(0, 4),
          probDist(0.0, 1.0) {}

    bool runOneExperiment() {
        int rifle = rifleDist(rng);

        double hitProbability;
        if (rifle < 3) {
            hitProbability = 0.95;
        } else {
            hitProbability = 0.70;
        }

        double x = probDist(rng);
        return x < hitProbability;
    }

    double runSeries(int experiments, int& hits) {
        hits = 0;

        for (int i = 0; i < experiments; ++i) {
            if (runOneExperiment()) {
                ++hits;
            }
        }

        return static_cast<double>(hits) / experiments;
    }
};

int main() {

    int experiments;
    std::cout << "Введите число экспериментов: ";
    std::cin >> experiments;

    if (experiments <= 0) {
        std::cout << "Число экспериментов должно быть положительным.\n";
        return 1;
    }

    ShootingSimulation simulation;

    int hits = 0;
    double empiricalProbability = simulation.runSeries(experiments, hits);

    const double theoreticalProbability = 3.0 / 5.0 * 0.95 + 2.0 / 5.0 * 0.70;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nРезультаты моделирования:\n";
    std::cout << "Количество экспериментов: " << experiments << "\n";
    std::cout << "Количество попаданий:     " << hits << "\n";
    std::cout << "Эмпирическая вероятность: " << empiricalProbability << "\n";
    std::cout << "Теоретическая вероятность:" << theoreticalProbability << "\n";
    std::cout << "Погрешность:              "
              << std::abs(empiricalProbability - theoreticalProbability) << "\n";

    return 0;
}