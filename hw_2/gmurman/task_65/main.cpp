#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int N = 100;
    const int WIN = 5;
    const int K = 2;
    const int trials = 1'000'000;

    int success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    for (int t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};

        int winChosen = 0;

        for (int step = 0; step < K; ++step) {
            int x;
            do {
                x = dist(rng);
            } while (used[x]);
            used[x] = true;

            if (x <= WIN) winChosen++;
        }

        if (winChosen == 2) success++;
    }

    double p_emp = static_cast<double>(success) / trials;

    // Теория: C(5,2)/C(100,2) = 1/495
    double p_theory = 1.0 / 495.0;

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Задача 65 (испытаний=" << trials << ")\n";
    std::cout << "Эмпирика: P ~ " << p_emp << "\n";
    std::cout << "Теория:   1/495 = " << p_theory << "\n";

    return 0;
}