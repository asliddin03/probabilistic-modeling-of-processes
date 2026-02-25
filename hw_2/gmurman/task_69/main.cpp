#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int N = 25;
    const int KNOWN = 20;
    const int K = 3;
    const int trials = 1'000'000;

    int success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    for (int t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};
        int knownChosen = 0;

        for (int step = 0; step < K; ++step) {
            int x;
            do {
                x = dist(rng);
            } while (used[x]);
            used[x] = true;

            if (x <= KNOWN) knownChosen++;
        }

        if (knownChosen == K) success++;
    }

    double p_emp = static_cast<double>(success) / trials;

    // Теория: C(20,3) / C(25,3) = 1140/2300 = 57/115
    double p_theory = 57.0 / 115.0;

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Задача 69 (испытаний=" << trials << ")\n";
    std::cout << "Эмпирика: P ~ " << p_emp << "\n";
    std::cout << "Теория:   57/115 = " << p_theory << "\n";

    return 0;
}