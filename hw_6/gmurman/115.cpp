#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;
    const double pBoy = 0.5;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution boy(pBoy);

    int countA = 0; // а) ровно 2 мальчика
    int countB = 0; // б) не более 2 мальчиков
    int countC = 0; // в) более 2 мальчиков
    int countD = 0; // г) не менее 2 и не более 3 мальчиков

    for (int i = 0; i < experiments; ++i) {
        int boys = 0;

        for (int j = 0; j < 5; ++j) {
            if (boy(gen)) {
                ++boys;
            }
        }

        if (boys == 2) {
            ++countA;
        }

        if (boys <= 2) {
            ++countB;
        }

        if (boys > 2) {
            ++countC;
        }

        if (boys >= 2 && boys <= 3) {
            ++countD;
        }
    }

    double empiricalA = static_cast<double>(countA) / experiments;
    double empiricalB = static_cast<double>(countB) / experiments;
    double empiricalC = static_cast<double>(countC) / experiments;
    double empiricalD = static_cast<double>(countD) / experiments;

    double theoreticalA = 5.0 / 16.0; // P(X = 2)
    double theoreticalB = 1.0 / 2.0;  // P(X <= 2)
    double theoreticalC = 1.0 / 2.0;  // P(X > 2)
    double theoreticalD = 5.0 / 8.0;  // P(2 <= X <= 3)

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Количество экспериментов: " << experiments << "\n\n";

    std::cout << "а) Два мальчика:\n";
    std::cout << "   Эмпирическая вероятность: " << empiricalA << "\n";
    std::cout << "   Теоретическая вероятность: " << theoreticalA << "\n\n";

    std::cout << "б) Не более двух мальчиков:\n";
    std::cout << "   Эмпирическая вероятность: " << empiricalB << "\n";
    std::cout << "   Теоретическая вероятность: " << theoreticalB << "\n\n";

    std::cout << "в) Более двух мальчиков:\n";
    std::cout << "   Эмпирическая вероятность: " << empiricalC << "\n";
    std::cout << "   Теоретическая вероятность: " << theoreticalC << "\n\n";

    std::cout << "г) Не менее двух и не более трех мальчиков:\n";
    std::cout << "   Эмпирическая вероятность: " << empiricalD << "\n";
    std::cout << "   Теоретическая вероятность: " << theoreticalD << "\n";

    return 0;
}