#include <iomanip>
#include <iostream>
#include <random>

using std::cout;

int main() {
    const int N  = 5;
    const int GOOD = 3;
    const int K = 2;
    const int trials = 1'000'000;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    int success = 0;
    for (int t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};
        int count = 0;
        for (int step = 0; step < K; ++step) {
            int x;
            while (true) {
                x = dist(rng);
                if (!used[x]) {
                    used[x] = true;
                    break;
                }
            }

            // Пусть элементы 1..3 — неизношенные, а 4..5 — изношенные
            if (x <= GOOD) {
                count++;
            }
        }

        if (count == K) {
            success++;
        }
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = 3.0 / 10.0; // C(3,2)/C(5,2) = 3/10

    cout << std::fixed << std::setprecision(6);
    cout << "Результаты моделирования (испытаний = " << trials << ")\n";
    cout << "P(оба неизношенные) ~ " << p << "\n";
    cout << "Теория: 3/10 = " << p_theory << "\n";

    return 0;
}