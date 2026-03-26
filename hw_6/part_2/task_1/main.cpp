#include <iostream>
#include <random>
#include <iomanip>
#include <cmath>

int main() {
    const int experiments = 1000000;

    int n;
    double p, p1;

    std::cout << "Введите число выстрелов n: ";
    std::cin >> n;
    std::cout << "Введите вероятность попадания p: ";
    std::cin >> p;
    std::cout << "Введите вероятность взрыва при одном попадании p1: ";
    std::cin >> p1;

    if (n <= 0 || p < 0.0 || p > 1.0 || p1 < 0.0 || p1 > 1.0) {
        std::cout << "Некорректные входные данные.\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    // Попадание пули в бочку
    std::bernoulli_distribution hit(p);

    // Взрыв при ровно одном попадании
    std::bernoulli_distribution explodeAfterOneHit(p1);

    int explosionCount = 0;

    for (int i = 0; i < experiments; ++i) {
        int hits = 0;

        for (int j = 0; j < n; ++j) {
            if (hit(gen)) {
                ++hits;
            }
        }

        bool exploded = false;

        if (hits >= 2) {
            exploded = true;
        } else if (hits == 1) {
            exploded = explodeAfterOneHit(gen);
        }

        if (exploded) {
            ++explosionCount;
        }
    }

    double empiricalProbability =
        static_cast<double>(explosionCount) / experiments;

    double theoreticalProbability =
        1.0 - std::pow(1.0 - p, n)
        - n * p * std::pow(1.0 - p, n - 1) * (1.0 - p1);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nКоличество экспериментов: " << experiments << "\n";
    std::cout << "Эмпирическая вероятность взрыва: " << empiricalProbability << "\n";
    std::cout << "Теоретическая вероятность взрыва: " << theoreticalProbability << "\n";

    return 0;
}