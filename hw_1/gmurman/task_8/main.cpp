#include <iomanip>
#include <iostream>
#include <random>

int main() {
    const int N = 6;
    const long long trials = 1'000'000;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    int good = 0;
    for (int t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};
        int last = 0;
        bool ok = true;
        for (int i = 1; i <= N; ++i) {
            int x;
            while (true) {
                x = dist(rng);
                if (!used[x]) {
                    used[x] = true;
                    break;
                }
            }

            if (x <= last) {
                ok = false;
                break;
            }
            last = x;
        }
        if (ok) {
            good++;
        }
    }

    double p = static_cast<double>(good) / trials;
    double p_theory = 1.0 / 720.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (n=6, испытаний=" << trials << ")\n";
    std::cout << "P(возрастающий порядок) ~ " << p << "\n";
    std::cout << "Теория: 1/720 = " << p_theory << "\n";
}
