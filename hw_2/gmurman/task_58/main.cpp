#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int trials = 1'000'000;

    int count_all_diff = 0;
    int count_at_least_pair = 0;
    int count_all_same = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 6);

    for (int t = 0; t < trials; ++t) {
        int a = dist(rng);
        int b = dist(rng);
        int c = dist(rng);

        bool all_same = (a == b && b == c);
        bool all_diff = (a != b && a != c && b != c);
        bool at_least_pair = !all_diff; // если не все разные то есть совпадение

        if (all_diff) count_all_diff++;
        if (at_least_pair) count_at_least_pair++;
        if (all_same) count_all_same++;
    }

    double pA = static_cast<double>(count_all_diff) / trials;
    double pB = static_cast<double>(count_at_least_pair) / trials;
    double pC = static_cast<double>(count_all_same) / trials;

    const double thA = 5.0 / 9.0;
    const double thB = 4.0 / 9.0;
    const double thC = 1.0 / 36.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Задача 58 (испытаний=" << trials << ")\n\n";

    std::cout << "a) Все разные:\n";
    std::cout << "Эмпирика: " << pA << "\n";
    std::cout << "Теория:   " << thA << "\n\n";

    std::cout << "б) Есть хотя бы одна пара одинаковых:\n";
    std::cout << "Эмпирика: " << pB << "\n";
    std::cout << "Теория:   " << thB << "\n\n";

    std::cout << "в) Все одинаковые:\n";
    std::cout << "Эмпирика: " << pC << "\n";
    std::cout << "Теория:   " << thC << "\n";

    return 0;
}