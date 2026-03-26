#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;

    std::random_device rd;
    std::mt19937 gen(rd());

    // Орел с вероятностью 1/2
    std::bernoulli_distribution eagle(0.5);

    int less_than_two = 0;     // орел выпал менее 2 раз
    int at_least_two = 0;      // орел выпал не менее 2 раз

    for (int i = 0; i < experiments; ++i) {
        int heads = 0;

        for (int j = 0; j < 5; ++j) {
            if (eagle(gen)) {
                ++heads;
            }
        }

        if (heads < 2) {
            ++less_than_two;
        }

        if (heads >= 2) {
            ++at_least_two;
        }
    }

    double p_less_than_two = static_cast<double>(less_than_two) / experiments;
    double p_at_least_two  = static_cast<double>(at_least_two) / experiments;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Количество экспериментов: " << experiments << "\n\n";

    std::cout << "Эмпирическая вероятность того, что орел выпадет менее 2 раз: "
              << p_less_than_two << "\n";
    std::cout << "Теоретическая вероятность: 0.187500\n\n";

    std::cout << "Эмпирическая вероятность того, что орел выпадет не менее 2 раз: "
              << p_at_least_two << "\n";
    std::cout << "Теоретическая вероятность: 0.812500\n";

    return 0;
}