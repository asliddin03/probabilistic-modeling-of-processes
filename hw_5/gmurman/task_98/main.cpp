#include <iostream>
#include <random>
#include <iomanip>

int main() {

    const int totalRifles = 10;
    const int scopedRifles = 4;
    const int regularRifles = 6;

    const double pHitIfScoped = 0.95;
    const double pHitIfRegular = 0.80;

    int experiments;
    std::cout << "Введите количество экспериментов: ";
    std::cin >> experiments;

    if (experiments <= 0) {
        std::cout << "Количество экспериментов должно быть больше нуля.\n";
        return 1;
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> rifleDist(1, totalRifles);
    std::uniform_real_distribution<double> probDist(0.0, 1.0);

    int totalHits = 0;
    int hitsFromScoped = 0;   // попадания из B1
    int hitsFromRegular = 0;  // попадания из B2

    for (int i = 0; i < experiments; ++i) {
        int rifle = rifleDist(gen);

        bool isScoped = (rifle <= scopedRifles);
        double pHit = isScoped ? pHitIfScoped : pHitIfRegular;

        bool hit = (probDist(gen) < pHit);

        if (hit) {
            ++totalHits;

            if (isScoped) {
                ++hitsFromScoped;
            } else {
                ++hitsFromRegular;
            }
        }
    }

    if (totalHits == 0) {
        std::cout << "Не было ни одного попадания.\n";
        return 0;
    }

    double empiricalP_B1_given_A = static_cast<double>(hitsFromScoped) / totalHits;
    double empiricalP_B2_given_A = static_cast<double>(hitsFromRegular) / totalHits;

    double pA =
        (static_cast<double>(scopedRifles) / totalRifles) * pHitIfScoped +
        (static_cast<double>(regularRifles) / totalRifles) * pHitIfRegular;

    double theoreticalP_B1_given_A =
        ((static_cast<double>(scopedRifles) / totalRifles) * pHitIfScoped) / pA;

    double theoreticalP_B2_given_A =
        ((static_cast<double>(regularRifles) / totalRifles) * pHitIfRegular) / pA;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nРезультаты моделирования:\n";
    std::cout << "Количество экспериментов: " << experiments << "\n";
    std::cout << "Количество попаданий:    " << totalHits << "\n";
    std::cout << "Попаданий из B1:         " << hitsFromScoped << "\n";
    std::cout << "Попаданий из B2:         " << hitsFromRegular << "\n";

    std::cout << "\nЭмпирические вероятности:\n";
    std::cout << "P(B1|A) = " << empiricalP_B1_given_A << "\n";
    std::cout << "P(B2|A) = " << empiricalP_B2_given_A << "\n";

    std::cout << "\nТеоретические вероятности:\n";
    std::cout << "P(B1|A) = " << theoreticalP_B1_given_A << "\n";
    std::cout << "P(B2|A) = " << theoreticalP_B2_given_A << "\n";

    std::cout << "\nАбсолютные ошибки:\n";
    std::cout << "|P(B1|A)_emp - P(B1|A)_theor| = "
              << std::abs(empiricalP_B1_given_A - theoreticalP_B1_given_A) << "\n";
    std::cout << "|P(B2|A)_emp - P(B2|A)_theor| = "
              << std::abs(empiricalP_B2_given_A - theoreticalP_B2_given_A) << "\n";

    return 0;
}