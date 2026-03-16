#include <iostream>
#include <random>
#include <iomanip>

int main() {

    const double pError1 = 0.05;
    const double pError2 = 0.10;

    int experiments;
    std::cout << "Введите количество экспериментов: ";
    std::cin >> experiments;

    if (experiments <= 0) {
        std::cout << "Количество экспериментов должно быть больше нуля.\n";
        return 1;
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> workerDist(1, 2);
    std::uniform_real_distribution<double> probDist(0.0, 1.0);

    int detectedErrors = 0;
    int errorsFromFirst = 0;

    for (int i = 0; i < experiments; ++i) {
        int worker = workerDist(gen);

        bool errorDetected = false;

        if (worker == 1) {
            errorDetected = (probDist(gen) < pError1);
            if (errorDetected) {
                ++detectedErrors;
                ++errorsFromFirst;
            }
        } else {
            errorDetected = (probDist(gen) < pError2);
            if (errorDetected) {
                ++detectedErrors;
            }
        }
    }

    if (detectedErrors == 0) {
        std::cout << "Ни одной ошибки не обнаружено, условную вероятность оценить нельзя.\n";
        return 0;
    }

    double empiricalP_B1_given_A =
        static_cast<double>(errorsFromFirst) / detectedErrors;

    double theoreticalP_B1_given_A =
        (0.5 * pError1) / (0.5 * pError1 + 0.5 * pError2);

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nРезультаты моделирования:\n";
    std::cout << "Количество экспериментов: " << experiments << "\n";
    std::cout << "Обнаружено ошибок:        " << detectedErrors << "\n";
    std::cout << "Ошибок 1-й перфораторщицы:" << errorsFromFirst << "\n";

    std::cout << "\nЭмпирическая вероятность:\n";
    std::cout << "P(B1|A) = " << empiricalP_B1_given_A << "\n";

    std::cout << "\nТеоретическая вероятность:\n";
    std::cout << "P(B1|A) = " << theoreticalP_B1_given_A << "\n";

    std::cout << "\nАбсолютная ошибка:\n";
    std::cout << "|P(B1|A)_эмп - P(B1|A)_теор| = "
              << std::abs(empiricalP_B1_given_A - theoreticalP_B1_given_A) << "\n";

    return 0;
}