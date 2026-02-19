#include <iostream>
#include <random>
#include <iomanip>

int main() {
    // n = сколько маленьких кубиков по ребру большого куба
    const int N = 10;

    const int trials = 1'000'000;

    int count1 = 0, count2 = 0, count3 = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, N);

    for (int t = 0; t < trials; ++t) {
        int i = dist(rng);
        int j = dist(rng);
        int k = dist(rng);

        int painted = 0;
        if (i == 1 || i == N) painted++;
        if (j == 1 || j == N) painted++;
        if (k == 1 || k == N) painted++;

        if (painted == 1) count1++;
        else if (painted == 2) count2++;
        else if (painted == 3) count3++;
    }

    double p1 = static_cast<double>(count1) / trials;
    double p2 = static_cast<double>(count2) / trials;
    double p3 = static_cast<double>(count3) / trials;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (n=10, испытаний=" << trials << ")\n";
    std::cout << "P(1 окрашенная грань) ~ " << p1 << "\n";
    std::cout << "P(2 окрашенные грани) ~ " << p2 << "\n";
    std::cout << "P(3 окрашенные грани) ~ " << p3 << "\n";

    return 0;
}
