#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const long long trials = 1'000'000;

    long long success = 0;

    std::mt19937 rng(std::random_device{}());

    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (long long t = 0; t < trials; ++t) {

        double x = dist(rng);
        double y = dist(rng);

        // Проверка условий:
        // x + y <= 1
        // xy >= 0.09
        if ((x + y <= 1.0) &&
            (x * y >= 0.09)) {
            success++;
            }
    }

    double p = static_cast<double>(success) / trials;

    double p_theory = 0.4 - 0.09 * std::log(9.0);

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Результаты моделирования\n";
    std::cout << "Количество испытаний: "<< trials << "\n\n";

    std::cout << "Экспериментальная вероятность: "<< p << "\n";

    std::cout << "Теоретическая вероятность:     "<< p_theory << "\n";

    return 0;
}