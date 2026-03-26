#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;

    std::random_device rd;
    std::mt19937 gen(rd());

    // Каждая точка попадает в одну из 4 равных частей с вероятностью 1/4
    std::uniform_int_distribution<int> partDist(0, 3);

    int successCount = 0; // в каждую часть попало ровно по 2 точки

    for (int i = 0; i < experiments; ++i) {
        int parts[4] = {0, 0, 0, 0};

        for (int j = 0; j < 8; ++j) {
            int part = partDist(gen);
            ++parts[part];
        }

        if (parts[0] == 2 && parts[1] == 2 &&
            parts[2] == 2 && parts[3] == 2) {
            ++successCount;
            }
    }

    double empiricalProbability =
        static_cast<double>(successCount) / experiments;

    double theoreticalProbability = 315.0 / 8192.0;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Количество экспериментов: " << experiments << "\n\n";
    std::cout << "Эмпирическая вероятность: " << empiricalProbability << "\n";
    std::cout << "Теоретическая вероятность: " << theoreticalProbability << "\n";

    return 0;
}