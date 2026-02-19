#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int N = 5;
    const int PAINTED = 3;
    const int K = 2;
    const long long trials = 1'000'000;

    long long c1 = 0, c2 = 0, cAtLeast1 = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    for (long long t = 0; t < trials; ++t) {
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

        if (paintedChosen == 1) c1++;
        if (paintedChosen == 2) c2++;
        if (paintedChosen >= 1) cAtLeast1++;
    }

    double p1 = static_cast<double>(c1) / trials;
    double p2 = static_cast<double>(c2) / trials;
    double pA = static_cast<double>(cAtLeast1) / trials;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (испытаний=" << trials << ")\n";
    std::cout << "P(ровно 1 окрашенное) ~ " << p1 << "  (теория 3/5 = " << 3.0/5.0 << ")\n";
    std::cout << "P(2 окрашенных)      ~ " << p2 << "  (теория 3/10 = " << 3.0/10.0 << ")\n";
    std::cout << "P(хотя бы 1)         ~ " << pA << "  (теория 9/10 = " << 9.0/10.0 << ")\n";

    return 0;
}
