#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;

    double a, x;
    std::cout << "Введите длину отрезка a: ";
    std::cin >> a;
    std::cout << "Введите x: ";
    std::cin >> x;

    if (a <= 0 || x < 0 || x > a) {
        std::cout << "Некорректные данные.\n";
        return 1;
    }

    double p = x / a; // точка попадет на расстояние меньше x от A

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution nearA(p);

    int successCount = 0; // ровно 2 точки ближе x к A

    for (int i = 0; i < experiments; ++i) {
        int countNearA = 0;

        for (int j = 0; j < 5; ++j) {
            if (nearA(gen)) {
                ++countNearA;
            }
        }

        if (countNearA == 2) {
            ++successCount;
        }
    }

    double empiricalProbability =
        static_cast<double>(successCount) / experiments;

    double theoreticalProbability = 10.0 * pow(x / a, 2) * pow((a - x) / a, 3);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nКоличество экспериментов: " << experiments << "\n";
    std::cout << "Эмпирическая вероятность: " << empiricalProbability << "\n";
    std::cout << "Теоретическая вероятность: " << theoreticalProbability << "\n";

    return 0;
}