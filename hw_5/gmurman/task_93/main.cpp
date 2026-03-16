#include <iostream>
#include <random>
#include <iomanip>

class DetailSimulation {
private:
    std::mt19937 gen;
    std::uniform_int_distribution<int> detailDist;   // 0..49
    std::uniform_real_distribution<double> probDist; // [0,1)

public:
    DetailSimulation()
        : gen(std::random_device{}()),
          detailDist(0, 49),
          probDist(0.0, 1.0) {}

    bool oneExperiment() {
        int detail = detailDist(gen);
        double excellentProbability = 0.0;

        if (detail <= 11) {
            excellentProbability = 0.9;
        } else if (detail <= 31) {
            excellentProbability = 0.6;
        } else {
            excellentProbability = 0.9;
        }

        double x = probDist(gen);
        return x < excellentProbability;
    }

    double run(int experiments, int& excellentCount) {
        excellentCount = 0;

        for (int i = 0; i < experiments; ++i) {
            if (oneExperiment()) {
                ++excellentCount;
            }
        }

        return static_cast<double>(excellentCount) / experiments;
    }
};

int main() {
    int n;
    std::cout << "Введите количество экспериментов: ";
    std::cin >> n;

    if (n <= 0) {
        std::cout << "Ошибка: количество экспериментов должно быть больше нуля.\n";
        return 1;
    }

    DetailSimulation simulation;

    int excellentCount = 0;
    double empirical = simulation.run(n, excellentCount);

    double theoretical =
        (12.0 / 50.0) * 0.9 +
        (20.0 / 50.0) * 0.6 +
        (18.0 / 50.0) * 0.9;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nРезультаты моделирования:\n";
    std::cout << "Количество экспериментов:        " << n << "\n";
    std::cout << "Отличных деталей:                " << excellentCount << "\n";
    std::cout << "Эмпирическая вероятность:        " << empirical << "\n";
    std::cout << "Теоретическая вероятность:       " << theoretical << "\n";
    std::cout << "Абсолютная погрешность:          "
              << std::abs(empirical - theoretical) << "\n";

    return 0;
}