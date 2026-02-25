#include <iostream>
#include <random>
#include <iomanip>

int main() { 
    const int N = 10;
    const int PAINTED = 6;
    const int K = 4;
    const int trials = 1'000'000;

    int success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    for (int t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};
        int paintedChosen = 0;

        for (int step = 0; step < K; ++step) {
            int x;
            do {
                x = dist(rng);
            } while (used[x]);
            used[x] = true;

            if (x <= PAINTED) paintedChosen++;
        }

        if (paintedChosen == K) success++;
    }

    double p_emp = static_cast<double>(success) / trials;

    // Теория: C(6,4) / C(10,4)
    // C(6,4) = 15, C(10,4) = 210 => 15/210 = 1/14
    double p_theory = 1.0 / 14.0;

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Задача 67 (испытаний=" << trials << ")\n";
    std::cout << "Эмпирика: P ~ " << p_emp << "\n";
    std::cout << "Теория:   1/14 = " << p_theory << "\n";

    return 0;
}