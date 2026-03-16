#include <iostream>
#include <random>
#include <iomanip>

int main() {

    const double pB1 = 0.55;
    const double pB2 = 0.45;

    const double pA_if_B1 = 0.90;
    const double pA_if_B2 = 0.98;

    int experiments;
    std::cout << "Введите количество экспериментов: ";
    std::cin >> experiments;

    if (experiments <= 0) {
        std::cout << "Количество экспериментов должно быть больше нуля.\n";
        return 1;
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    int standardCount = 0;          // сколько изделий признано стандартными
    int standardBySecondCount = 0;  // сколько из них проверил 2-й товаровед

    for (int i = 0; i < experiments; ++i) {
        double x = dist(gen);

        bool checkedBySecond = false;
        double pStandard = 0.0;

        if (x < pB1) {
            checkedBySecond = false;
            pStandard = pA_if_B1;
        } else {
            checkedBySecond = true;
            pStandard = pA_if_B2;
        }

        bool isStandard = (dist(gen) < pStandard);

        if (isStandard) {
            ++standardCount;

            if (checkedBySecond) {
                ++standardBySecondCount;
            }
        }
    }

    if (standardCount == 0) {
        std::cout << "Не было изделий, признанных стандартными.\n";
        return 0;
    }

    double empirical = static_cast<double>(standardBySecondCount) / standardCount;

    double pA = pB1 * pA_if_B1 + pB2 * pA_if_B2;
    double theoretical = (pB2 * pA_if_B2) / pA;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nРезультаты моделирования:\n";
    std::cout << "Количество экспериментов:                " << experiments << "\n";
    std::cout << "Изделий признано стандартными:           " << standardCount << "\n";
    std::cout << "Из них проверено 2-м товароведом:        " << standardBySecondCount << "\n";

    std::cout << "\nЭмпирическая вероятность P(B2|A):        " << empirical << "\n";
    std::cout << "Теоретическая вероятность P(B2|A):       " << theoretical << "\n";
    std::cout << "Абсолютная ошибка:                       "
              << std::abs(empirical - theoretical) << "\n";

    return 0;
}