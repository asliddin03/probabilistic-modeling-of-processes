#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const double a = 10.0;
    const double r = 3.0;          // r < a
    const long long trials = 1'000'000;

    long long success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 2.0 * a);

    for (long long t = 0; t < trials; ++t) {
        double x = dist(rng);

        if (x >= r && x <= 2.0 * a - r) {
            success++;
        }
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = 1.0 - r / a;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (испытаний=" << trials << ")\n";
    std::cout << "a=" << a << ", r=" << r << "\n";
    std::cout << "Эксперимент: P ~ " << p << "\n";
    std::cout << "Теория:   1 - r/a = " << p_theory << "\n";

    return 0;
}
