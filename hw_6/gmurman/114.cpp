#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;
    const double failProbability = 0.1;
    const double workProbability = 1.0 - failProbability;

    std::random_device rd;
    std::mt19937 gen(rd());

    // true -> элемент работает
    std::bernoulli_distribution elementWorks(workProbability);

    int successA = 0; // а) работают только 3 основных элемента
    int successB = 0; // б) работают 3 основных + 1 резервный
    int successC = 0; // в) работают 3 основных + 2 резервных

    for (int i = 0; i < experiments; ++i) {
        // а) всего 3 элемента, должны работать все 3
        int workingA = 0;
        for (int j = 0; j < 3; ++j) {
            if (elementWorks(gen)) {
                ++workingA;
            }
        }
        if (workingA >= 3) {
            ++successA;
        }

        // б) всего 4 элемента, устройство работает если исправны не менее 3
        int workingB = 0;
        for (int j = 0; j < 4; ++j) {
            if (elementWorks(gen)) {
                ++workingB;
            }
        }
        if (workingB >= 3) {
            ++successB;
        }

        // в) всего 5 элементов, устройство работает если исправны не менее 3
        int workingC = 0;
        for (int j = 0; j < 5; ++j) {
            if (elementWorks(gen)) {
                ++workingC;
            }
        }
        if (workingC >= 3) {
            ++successC;
        }
    }

    double empiricalA = static_cast<double>(successA) / experiments;
    double empiricalB = static_cast<double>(successB) / experiments;
    double empiricalC = static_cast<double>(successC) / experiments;

    double theoreticalA = 0.72900;
    double theoreticalB = 0.94770;
    double theoreticalC = 0.99144;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Количество экспериментов: " << experiments << "\n\n";

    std::cout << "а) Только 3 основных элемента\n";
    std::cout << "   Эмпирическая вероятность безотказной работы: " << empiricalA << "\n";
    std::cout << "   Теоретическая вероятность: " << theoreticalA << "\n\n";

    std::cout << "б) 3 основных и 1 резервный элемент\n";
    std::cout << "   Эмпирическая вероятность безотказной работы: " << empiricalB << "\n";
    std::cout << "   Теоретическая вероятность: " << theoreticalB << "\n\n";

    std::cout << "в) 3 основных и 2 резервных элемента\n";
    std::cout << "   Эмпирическая вероятность безотказной работы: " << empiricalC << "\n";
    std::cout << "   Теоретическая вероятность: " << theoreticalC << "\n";

    return 0;
}