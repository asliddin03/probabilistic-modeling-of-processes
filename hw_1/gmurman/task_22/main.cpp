#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int POS = 5;
    const long long trials = 1'000'000;

    // Секретная комбинация
    const int secret[4] = {1, 2, 3, 4};

    long long success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, POS);

    for (long long t = 0; t < trials; ++t) {
        int a = dist(rng);
        int b = dist(rng);
        int c = dist(rng);
        int d = dist(rng);

        if (a == secret[0] && b == secret[1] && c == secret[2] && d == secret[3]) {
            success++;
        }
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = 1.0 / 625.0; // 5^4 = 625

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Результаты моделирования (испытаний=" << trials << ")\n";
    std::cout << "P(открыть замок) ~ " << p << "\n";
    std::cout << "Теория: 1/625 = " << p_theory << "\n";

    return 0;
}
