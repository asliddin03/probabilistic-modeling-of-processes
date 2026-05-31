#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const double a = 10.0;

    const double r = 2.0;

    const long long trials = 1'000'000;

    long long success = 0;

    // Генератор случайных чисел
    std::mt19937 rng(std::random_device{}());

    // Координаты центра монеты внутри квадрата
    std::uniform_real_distribution<double> dist(0.0, a);

    for (long long t = 0; t < trials; ++t) {

        double x = dist(rng);
        double y = dist(rng);

        // Монета не пересекает стороны квадрата,
        // если центр находится не ближе r к границам
        if (x >= r && x <= a - r &&
            y >= r && y <= a - r) {

            success++;
            }
    }

    // Экспериментальная вероятность
    double p = static_cast<double>(success) / trials;

    // Теоретическая вероятность
    double p_theory = ((a - 2.0 * r) * (a - 2.0 * r)) / (a * a);

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Результаты моделирования\n";
    std::cout << "Количество испытаний: " << trials << "\n";
    std::cout << "a = " << a << "\n";
    std::cout << "r = " << r << "\n\n";

    std::cout << "Экспериментальная вероятность: " << p << "\n";
    std::cout << "Теоретическая вероятность:     " << p_theory << "\n";

    return 0;
}