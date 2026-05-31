#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const double r1 = 5.0;
    const double r2 = 10.0;

    const long long trials = 1'000'000;

    long long success = 0;

    std::mt19937 rng(std::random_device{}());

    std::uniform_real_distribution<double> dist(-r2, r2);

    for (long long t = 0; t < trials; ++t) {

        double x = dist(rng);
        double y = dist(rng);

        // Расстояние от центра
        double d2 = x * x + y * y;

        // Проверяем:
        // точка попала в большой круг
        if (d2 <= r2 * r2) {

            // и одновременно попала в кольцо
            if (d2 >= r1 * r1) {
                success++;
            }
        }
    }

    double p = static_cast<double>(success) / trials;

    double p_theory = (M_PI * r2 * r2 - M_PI * r1 * r1) / (M_PI * r2 * r2);

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Результаты моделирования\n";
    std::cout << "Количество испытаний: " << trials << "\n";
    std::cout << "r1 = " << r1 << "\n";
    std::cout << "r2 = " << r2 << "\n\n";

    std::cout << "Экспериментальная вероятность: "<< p << "\n";

    std::cout << "Теоретическая вероятность:     "<< p_theory << "\n";

    return 0;
}