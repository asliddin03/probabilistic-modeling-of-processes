#include <iostream>
#include <random>
#include <iomanip>
#include <algorithm>

int main() {
    const int N = 5;
    const int K = 3;
    const int trials = 1'000'000;

    int successA = 0;  // (1,4,5) именно в таком порядке
    int successB = 0;  // {1,4,5} в любом порядке

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

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

        // a) строго 1, потом 4, потом 5
        if (x1 == 1 && x2 == 4 && x3 == 5) {
            successA++;
        }

        // b) набор {1,4,5} в любом порядке
        int arr[3] = {x1, x2, x3};
        std::sort(arr, arr + 3);
        if (arr[0] == 1 && arr[1] == 4 && arr[2] == 5) {
            successB++;
        }
    }

    double pA_emp = static_cast<double>(successA) / trials;
    double pB_emp = static_cast<double>(successB) / trials;

    // a) 1/60
    // b) 1/10
    double pA_theory = 1.0 / 60.0;
    double pB_theory = 1.0 / 10.0;

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Задача 68 (испытаний=" << trials << ")\n\n";

    std::cout << "a) P(1 затем 4 затем 5)\n";
    std::cout << "Эмпирика: " << pA_emp << "\n";
    std::cout << "Теория:   1/60 = " << pA_theory << "\n\n";

    std::cout << "b) P(1,4,5 в любом порядке)\n";
    std::cout << "Эмпирика: " << pB_emp << "\n";
    std::cout << "Теория:   1/10 = " << pB_theory << "\n";

    return 0;
}