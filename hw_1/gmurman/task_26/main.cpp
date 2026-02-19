#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const double L = 20.0;
    const double l = 10.0;
    const long long trials = 1'000'000;

    const double a = 0.0;         // начало малого
    const double b = a + l;       // конец малого

    long long success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, L);

    for (long long t = 0; t < trials; ++t) {
        double x = dist(rng);
        if (x >= a && x <= b) success++;
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = l / L; // 10/20 = 0.5

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (испытаний=" << trials << ")\n";
    std::cout << "Эксперимент: P ~ " << p << "\n";
    std::cout << "Теория:   10/20 = " << p_theory << "\n";
    return 0;
}
