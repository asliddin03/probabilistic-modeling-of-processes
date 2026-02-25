#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int N = 10;
    const int K = 3;
    const int trials = 1'000'000;

    int successA = 0;
    int successB = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    // (a) с возвращением
    for (int t = 0; t < trials; ++t) {
        int x1 = dist(rng);
        int x2 = dist(rng);
        int x3 = dist(rng);

        if (x1 == 1 && x2 == 2 && x3 == 3) successA++;
    }

    // (б) без возвращения
    for (int t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};
        int x1 = 0, x2 = 0, x3 = 0;

        for (int step = 0; step < K; ++step) {
            int x;
            do {
                x = dist(rng);
            } while (used[x]);
            used[x] = true;

            if (step == 0) x1 = x;
            if (step == 1) x2 = x;
            if (step == 2) x3 = x;
        }

        if (x1 == 1 && x2 == 2 && x3 == 3) successB++;
    }

    double pA_emp = static_cast<double>(successA) / trials;
    double pB_emp = static_cast<double>(successB) / trials;

    double pA_theory = 1.0 / 1000.0;
    double pB_theory = 1.0 / 720.0;

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Задача 70 (испытаний=" << trials << ")\n\n";

    std::cout << "a) С возвращением: P(1,2,3)\n";
    std::cout << "Эмпирика: " << pA_emp << "\n";
    std::cout << "Теория:   1/1000 = " << pA_theory << "\n\n";

    std::cout << "б) Без возвращения: P(1,2,3)\n";
    std::cout << "Эмпирика: " << pB_emp << "\n";
    std::cout << "Теория:   1/720  = " << pB_theory << "\n";

    return 0;
}