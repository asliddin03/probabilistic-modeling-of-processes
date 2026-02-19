#include <iomanip>
#include <iostream>
#include <random>

using std::cout;

int main() {
    const int N = 15;
    const int K = 5;
    const int trials = 1'000'000;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);
    int success = 0;
    for (int t = 0; t < trials; ++t) {
        bool used[N + 1] = {false};
        int good = 0;
        for (int i = 0; i < K; ++i) {
            int x;
            while (true) {
                x = dist(rng);
                if (!used[x]) {
                    used[x] = true;
                    break;
                }
            }

            // Пусть кинескопы 1, ... , 10 львовские
            if (x <= 10) {
                good++;
            }
        }
        if (good == 3) {
            success++;
        }
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = 400.0 / 1001.0; // C(3,10)*C(2, 5)/C(5,15) = 400/1001

    cout << std::fixed << std::setprecision(6);
    cout << "Результаты моделирования (испытаний = " << trials << ")\n";
    cout << "P(3 львовские) ~ " << p << "\n";
    cout << "Теория: 400/1001 = " << p_theory << "\n";
}