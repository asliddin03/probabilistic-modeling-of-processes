#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const double p1 = 0.6, p2 = 0.7, p3 = 0.8, p4 = 0.9;
    const int trials = 1'000'000;

    long long count_le3 = 0;  // X <= 3
    long long count_ge2 = 0;  // X >= 2

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int t = 0; t < trials; ++t) {
        int x = 0;

        if (dist(rng) < p1) x++;
        if (dist(rng) < p2) x++;
        if (dist(rng) < p3) x++;
        if (dist(rng) < p4) x++;

        if (x <= 3) count_le3++;
        if (x >= 2) count_ge2++;
    }

    double p_emp_le3 = static_cast<double>(count_le3) / trials;
    double p_emp_ge2 = static_cast<double>(count_ge2) / trials;

    double p_all4 = p1 * p2 * p3 * p4;         // P(X=4)
    double p_theory_le3 = 1.0 - p_all4;        // P(X<=3)

    double q1 = 1.0 - p1, q2 = 1.0 - p2, q3 = 1.0 - p3, q4 = 1.0 - p4;
    double p0 = q1 * q2 * q3 * q4;             // P(X=0)
    double p1only =
        p1 * q2 * q3 * q4 +
        p2 * q1 * q3 * q4 +
        p3 * q1 * q2 * q4 +
        p4 * q1 * q2 * q3;                      // P(X=1)
    double p_theory_ge2 = 1.0 - p0 - p1only;    // P(X>=2)

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Задача 57 (испытаний=" << trials << ")\n\n";

    std::cout << "a) P(X <= 3)\n";
    std::cout << "Эмпирика: " << p_emp_le3 << "\n";
    std::cout << "Теория:   " << p_theory_le3 << "\n\n";

    std::cout << "b) P(X >= 2)\n";
    std::cout << "Эмпирика: " << p_emp_ge2 << "\n";
    std::cout << "Теория:   " << p_theory_ge2 << "\n";

    return 0;
}