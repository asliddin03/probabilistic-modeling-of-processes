#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const double p = 0.8;
    const double limit = 0.4;
    const long long trials = 1'000'000;

    // 1) Подбор максимального n по формуле: 1 - p^n < limit
    int best_n = 0;
    for (int n = 1; n <= 1000; ++n) {
        double prob_at_least_one_miss = 1.0 - std::pow(p, n);
        if (prob_at_least_one_miss < limit) {
            best_n = n;
        } else {
            break;
        }
    }

    double theory = 1.0 - std::pow(p, best_n);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    long long count_at_least_one_miss = 0;

    for (long long t = 0; t < trials; ++t) {
        bool miss_happened = false;

        for (int i = 0; i < best_n; ++i) {
            bool hit = (dist(rng) < p);
            if (!hit) {
                miss_happened = true;
                break;
            }
        }

        if (miss_happened) count_at_least_one_miss++;
    }

    double empirical = static_cast<double>(count_at_least_one_miss) / trials;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "p = " << p << ", требование: P(хотя бы один промах) < " << limit << "\n";
    std::cout << "Максимальное n = " << best_n << "\n\n";
    std::cout << "Теория:   1 - p^n = " << theory << "\n";
    std::cout << "Эмпирика: ~ " << empirical << " (испытаний " << trials << ")\n";

    return 0;
}