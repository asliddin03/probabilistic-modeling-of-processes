#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;

    std::random_device rd;
    std::mt19937 gen(rd());

    // a: p = 0.4, 4 испытания, нужно X >= 3
    std::bernoulli_distribution eventA(0.4);

    // б: p = 0.8, 5 испытаний, нужно X >= 4
    std::bernoulli_distribution eventB(0.8);

    int countA = 0;
    int countB = 0;

    for (int i = 0; i < experiments; ++i) {
        int successesA = 0;
        for (int j = 0; j < 4; ++j) {
            if (eventA(gen)) {
                ++successesA;
            }
        }
        if (successesA >= 3) {
            ++countA;
        }

        int successesB = 0;
        for (int j = 0; j < 5; ++j) {
            if (eventB(gen)) {
                ++successesB;
            }
        }
        if (successesB >= 4) {
            ++countB;
        }
    }

    double empiricalA = static_cast<double>(countA) / experiments;
    double empiricalB = static_cast<double>(countB) / experiments;

    double theoreticalA = 0.1792;
    double theoreticalB = 0.73728;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Количество экспериментов: " << experiments << "\n\n";

    std::cout << "113a) Эмпирическая вероятность P(X >= 3), p = 0.4, n = 4: "
              << empiricalA << "\n";
    std::cout << "     Теоретическая вероятность: " << theoreticalA << "\n\n";

    std::cout << "113б) Эмпирическая вероятность P(X >= 4), p = 0.8, n = 5: "
              << empiricalB << "\n";
    std::cout << "     Теоретическая вероятность: " << theoreticalB << "\n";

    return 0;
}