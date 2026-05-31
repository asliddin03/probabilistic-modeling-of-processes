#include <iostream>
#include <random>
#include <iomanip>

struct Result {
    double lessThanK;
    double evenLength;
};

Result empirical(int k, long long experiments) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> coin(0, 1);

    long long lessThanK = 0;
    long long evenLength = 0;

    for (long long exp = 0; exp < experiments; exp++) {
        int prev = coin(gen);
        int throwsCount = 1;

        while (true) {
            int current = coin(gen);
            throwsCount++;

            if (current == prev)
                break;

            prev = current;
        }

        if (throwsCount < k)
            lessThanK++;

        if (throwsCount % 2 == 0)
            evenLength++;
    }

    return {
        static_cast<double>(lessThanK) / experiments,
        static_cast<double>(evenLength) / experiments
    };
}

Result analytical(int k) {
    return {
        1.0 - 1.0 / std::pow(2.0, k - 2),2.0 / 3.0
    };
}

void printResults(const Result& emp, const Result& ana) {
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nСобытие: эксперимент закончится до k-го броска\n";
    std::cout << "Эмпирическая вероятность: " << emp.lessThanK << '\n';
    std::cout << "Аналитическая вероятность: " << ana.lessThanK << "\n\n";

    std::cout << "Событие: потребуется четное число бросков\n";
    std::cout << "Эмпирическая вероятность: " << emp.evenLength << '\n';
    std::cout << "Аналитическая вероятность: " << ana.evenLength << '\n';
}

int main() {
    int k;
    long long experiments;

    std::cout << "Введите k: ";
    if (!(std::cin >> k) || k < 2) {
        std::cout << "Некорректное значение k\n";
        return 0;
    }

    std::cout << "Введите количество экспериментов: ";
    if (!(std::cin >> experiments) || experiments <= 0) {
        std::cout << "Некорректное количество экспериментов\n";
        return 0;
    }

    Result emp = empirical(k, experiments);
    Result ana = analytical(k);

    printResults(emp, ana);

    return 0;
}