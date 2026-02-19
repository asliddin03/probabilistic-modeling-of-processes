#include <iomanip>
#include <iostream>
#include <random>

int main() {
    const int N = 100;
    const int K = 10;
    const int SPECIAL = 1;

    const int trials = 1'000'000;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    int success = 0;
    for (int t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};
        bool found = false;
        for (int i = 0; i < K; ++i) {
            int x;
            while (true) {
                x = dist(rng);
                if (!used[x]) {
                    used[x] = true;
                    break;
                }
            }
            if (x == SPECIAL) {
                found = true;
                break;
            }
        }

        if (found) {
            success++;
        }
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = static_cast<double>(K) / N;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (N=100, испытаний=" << trials << ")\n";
    std::cout << "P(особая среди " << K << " из " << N << ") ~ " << p << "\n";
    std::cout << "Теория: " << K << "/" << N << " = " << p_theory << "\n";
}