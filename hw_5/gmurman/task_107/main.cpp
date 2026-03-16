#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const double p1 = 0.6;
    const double p2 = 0.5;
    const double p3 = 0.4;

    int experiments;
    std::cout << "Введите количество экспериментов: ";
    std::cin >> experiments;

    if (experiments <= 0) {
        std::cout << "Количество экспериментов должно быть больше нуля.\n";
        return 1;
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    int countA = 0;      // сколько раз было ровно 2 попадания
    int countAB = 0;     // сколько раз было ровно 2 попадания и попал 3-й стрелок

    for (int i = 0; i < experiments; ++i) {
        bool hit1 = (dist(gen) < p1);
        bool hit2 = (dist(gen) < p2);
        bool hit3 = (dist(gen) < p3);

        int totalHits = static_cast<int>(hit1) + static_cast<int>(hit2) + static_cast<int>(hit3);

        if (totalHits == 2) {
            ++countA;

            if (hit3) {
                ++countAB;
            }
        }
    }

    if (countA == 0) {
        std::cout << "Ни разу не получилось ровно двух попаданий.\n";
        return 0;
    }

    double empirical = static_cast<double>(countAB) / countA;
    double theoretical = 10.0 / 19.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nРезультаты моделирования:\n";
    std::cout << "Количество экспериментов:                " << experiments << "\n";
    std::cout << "Случаев с ровно двумя попаданиями:       " << countA << "\n";
    std::cout << "Из них попал 3-й стрелок:                " << countAB << "\n";
    std::cout << "Эмпирическая вероятность P(B|A):         " << empirical << "\n";
    std::cout << "Теоретическая вероятность P(B|A):        " << theoretical << "\n";
    std::cout << "Абсолютная ошибка:                       "
              << std::abs(empirical - theoretical) << "\n";

    return 0;
}