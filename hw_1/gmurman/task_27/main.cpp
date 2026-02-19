#include <iostream>
#include <random>
#include <iomanip>
#include <algorithm>

int main() {
    const double L = 30.0;
    const long long trials = 1'000'000;

    long long success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, L);

    for (long long t = 0; t < trials; ++t) {
        double x = dist(rng);
        double ob = x;
        double ba = L - x;

        if (std::min(ob, ba) > L / 3.0) success++;
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = 1.0 / 3.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (испытаний=" << trials << ", L=" << L << ")\n";
    std::cout << "Эксперимент: P ~ " << p << "\n";
    std::cout << "Теория:   1/3 = " << p_theory << "\n";
    return 0;
}
