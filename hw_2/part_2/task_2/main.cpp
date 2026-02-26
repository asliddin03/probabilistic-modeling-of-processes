#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

static void usage(const char* prog) {
    std::cerr << "Использование:\n"
              << "  " << prog << " <n> <K>\n\n"
              << "Где:\n"
              << "  n  - число ключей (n >= 1)\n"
              << "  K  - число экспериментов (K >= 1)\n\n"
              << "Пример:\n"
              << "  " << prog << " 10 20\n";
}

int main(int argc, char** argv) {
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }

    long long n = 0, K = 0;
    try {
        n = std::stoll(argv[1]);
        K = std::stoll(argv[2]);
    } catch (...) {
        usage(argv[0]);
        return 1;
    }

    if (n < 1 || K < 1) {
        usage(argv[0]);
        return 1;
    }

    // ГСЧ
    std::mt19937 rng(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    ));

    // Ключи 1..n, правильный пусть будет 1
    const int correct = 1;
    std::vector<int> keys(static_cast<size_t>(n));
    std::iota(keys.begin(), keys.end(), 1);

    // counts[k] = сколько раз закончилось на (k+1)-й попытке
    std::vector<long long> counts(static_cast<size_t>(n), 0);

    std::cout << "Моделирование связки ключей: n=" << n << ", K=" << K << "\n";
    std::cout << "Правильный ключ: " << correct << "\n\n";

    for (long long exp = 1; exp <= K; ++exp) {
        std::shuffle(keys.begin(), keys.end(), rng);

        auto it = std::find(keys.begin(), keys.end(), correct);
        long long pos = std::distance(keys.begin(), it);
        long long attempts = pos + 1;

        counts[static_cast<size_t>(attempts - 1)]++;

        std::cout << "Эксперимент " << exp
                  << ": подошёл на попытке " << attempts << "\n";
    }

    std::cout << "\nЭмпирические вероятности P(T=k):\n";
    std::cout << std::fixed << std::setprecision(6);

    for (long long k = 1; k <= n; ++k) {
        double p_emp = static_cast<double>(counts[static_cast<size_t>(k - 1)]) /
                       static_cast<double>(K);
        double p_theory = 1.0 / static_cast<double>(n);

        std::cout << "k=" << k
                  << "  эмпирика=" << p_emp
                  << "  теория=1/n=" << p_theory
                  << "  (count=" << counts[static_cast<size_t>(k - 1)] << ")\n";
    }

    return 0;
}