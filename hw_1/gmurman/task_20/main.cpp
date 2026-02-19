#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int N = 12;
    const int EXCELLENT = 8;
    const int K = 9;
    const long long trials = 1'000'000;

    long long success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    for (long long t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};
        int excellentChosen = 0;

        for (int step = 0; step < K; ++step) {
            int x;
            do {
                x = dist(rng);
            } while (used[x]);
            used[x] = true;

            if (x <= EXCELLENT) excellentChosen++;
        }

        if (excellentChosen == 5) success++;
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = 14.0 / 55.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (испытаний=" << trials << ")\n";
    std::cout << "P(ровно 5 отличников среди 9) ~ " << p << "\n";
    std::cout << "Теория: 14/55 = " << p_theory << "\n";

    return 0;
}
