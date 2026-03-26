#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;

    int n, k;
    std::cout << "Введите общее число монет n: ";
    std::cin >> n;
    std::cout << "Введите число монет с двумя орлами k: ";
    std::cin >> k;

    if (n <= 0 || k < 0 || k > n) {
        std::cout << "Некорректные данные.\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    // Выбор монеты: числа 0..k-1 считаем двусторонне-орловыми
    std::uniform_int_distribution<int> chooseCoin(0, n - 1);

    // Для обычной монеты: орёл с вероятностью 1/2
    std::bernoulli_distribution fairHead(0.5);

    long long firstThreeHeadsCount = 0;
    long long firstThreeAndFourthHeadCount = 0;

    for (int i = 0; i < experiments; ++i) {
        int coinIndex = chooseCoin(gen);
        bool doubleHeaded = (coinIndex < k);

        bool toss[4];

        for (int j = 0; j < 4; ++j) {
            if (doubleHeaded) {
                toss[j] = true; // орёл всегда
            } else {
                toss[j] = fairHead(gen);
            }
        }

        if (toss[0] && toss[1] && toss[2]) {
            ++firstThreeHeadsCount;

            if (toss[3]) {
                ++firstThreeAndFourthHeadCount;
            }
        }
    }

    double empiricalProbability = 0.0;
    if (firstThreeHeadsCount > 0) {
        empiricalProbability =
            static_cast<double>(firstThreeAndFourthHeadCount) / firstThreeHeadsCount;
    }

    double theoreticalProbability =
        static_cast<double>(n + 15 * k) / (2.0 * (n + 7 * k));

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nКоличество экспериментов: " << experiments << "\n";
    std::cout << "Случаев, когда в первых трех бросках выпал орел: "
              << firstThreeHeadsCount << "\n";
    std::cout << "Случаев, когда и в 4-м броске выпал орел: "
              << firstThreeAndFourthHeadCount << "\n\n";
    std::cout << "Эмпирическая условная вероятность: "
              << empiricalProbability << "\n";
    std::cout << "Теоретическая условная вероятность: "
              << theoreticalProbability << "\n";

    return 0;
}