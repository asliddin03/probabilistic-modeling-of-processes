#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int trials = 1'000'000;

    int success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int t = 0; t < trials; ++t) {
        int c1 = 0, c2 = 0, c3 = 0;

        // бросаем 3 точки
        for (int i = 0; i < 3; ++i) {
            double x = dist(rng);

            // определяем, в какую треть попала
            if (x < 1.0 / 3.0) c1++;
            else if (x < 2.0 / 3.0) c2++;
            else c3++;
        }

        if (c1 <= 1 && c2 <= 1 && c3 <= 1) {
            success++;
        }
    }

    double p_emp = static_cast<double>(success) / trials;
    double p_theory = 2.0 / 9.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Задача 63 (испытаний=" << trials << ")\n";
    std::cout << "Эмпирика: P ~ " << p_emp << "\n";
    std::cout << "Теория:   2/9 = " << p_theory << "\n";

    return 0;
}