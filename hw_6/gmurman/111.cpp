#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;

    std::random_device rd;
    std::mt19937 gen(rd());

    // Победа с вероятностью 1/2, проигрыш с вероятностью 1/2
    std::bernoulli_distribution win(0.5);

    int one_of_two = 0;
    int two_of_four = 0;
    int at_least_two_of_four = 0;
    int at_least_three_of_five = 0;

    for (int i = 0; i < experiments; ++i) {
        // 1) Выиграть 1 партию из 2
        int wins2 = 0;
        for (int j = 0; j < 2; ++j) {
            if (win(gen)) {
                ++wins2;
            }
        }
        if (wins2 == 1) {
            ++one_of_two;
        }

        // 2) Выиграть 2 партии из 4
        int wins4 = 0;
        for (int j = 0; j < 4; ++j) {
            if (win(gen)) {
                ++wins4;
            }
        }
        if (wins4 == 2) {
            ++two_of_four;
        }
        if (wins4 >= 2) {
            ++at_least_two_of_four;
        }

        // 3) Выиграть не менее 3 партий из 5
        int wins5 = 0;
        for (int j = 0; j < 5; ++j) {
            if (win(gen)) {
                ++wins5;
            }
        }
        if (wins5 >= 3) {
            ++at_least_three_of_five;
        }
    }

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Количество экспериментов: " << experiments << "\n\n";

    std::cout << "1) Эмпирическая вероятность выиграть 1 партию из 2: "
              << static_cast<double>(one_of_two) / experiments << "\n";
    std::cout << "   Теоретическая: 0.500000\n\n";

    std::cout << "2) Эмпирическая вероятность выиграть 2 партии из 4: "
              << static_cast<double>(two_of_four) / experiments << "\n";
    std::cout << "   Теоретическая: 0.375000\n\n";

    std::cout << "3) Эмпирическая вероятность выиграть не менее 2 партий из 4: "
              << static_cast<double>(at_least_two_of_four) / experiments << "\n";
    std::cout << "   Теоретическая: 0.687500\n\n";

    std::cout << "4) Эмпирическая вероятность выиграть не менее 3 партий из 5: "
              << static_cast<double>(at_least_three_of_five) / experiments << "\n";
    std::cout << "   Теоретическая: 0.500000\n";

    return 0;
}