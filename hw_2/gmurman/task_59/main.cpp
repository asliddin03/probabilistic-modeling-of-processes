#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int trials = 1'000'000;

    int count_a = 0;
    int count_b = 0;
    int count_v = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 6);

    for (int t = 0; t < trials; ++t) {
        int x1 = dist(rng);
        int x2 = dist(rng);
        int x3 = dist(rng);

        // (a): ровно две "1" и третья != 1
        int ones = (x1 == 1) + (x2 == 1) + (x3 == 1);
        if (ones == 2) count_a++;

        // (б): ровно две одинаковые, третья другая
        bool all_same = (x1 == x2 && x2 == x3);
        bool all_diff = (x1 != x2 && x1 != x3 && x2 != x3);
        bool exactly_one_pair = (!all_same && !all_diff);
        if (exactly_one_pair) count_b++;

        // (в): все разные
        if (all_diff) count_v++;
    }

    double p_a = static_cast<double>(count_a) / trials;
    double p_b = static_cast<double>(count_b) / trials;
    double p_v = static_cast<double>(count_v) / trials;

    const double th_a = 5.0 / 72.0;
    const double th_b = 5.0 / 12.0;
    const double th_v = 5.0 / 9.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Задача 59 (испытаний=" << trials << ")\n\n";

    std::cout << "a) Две единицы, третья не 1:\n";
    std::cout << "Эмпирика: " << p_a << "\n";
    std::cout << "Теория:   5/72 = " << th_a << "\n\n";

    std::cout << "б) Ровно две одинаковые, третья другая:\n";
    std::cout << "Эмпирика: " << p_b << "\n";
    std::cout << "Теория:   5/12 = " << th_b << "\n\n";

    std::cout << "в) Все разные:\n";
    std::cout << "Эмпирика: " << p_v << "\n";
    std::cout << "Теория:   5/9 = " << th_v << "\n";

    return 0;
}