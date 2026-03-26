#include <iostream>
#include <random>
#include <iomanip>

int main() {
    const int experiments = 1000000;
    const double pLeft = 2.0 / 3.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution leftOfC(pLeft);

    int successCount = 0; // ровно 2 точки левее C и 2 правее C

    for (int i = 0; i < experiments; ++i) {
        int leftCount = 0;

        for (int j = 0; j < 4; ++j) {
            if (leftOfC(gen)) {
                ++leftCount;
            }
        }

        if (leftCount == 2) {
            ++successCount;
        }
    }

    double empiricalProbability =
        static_cast<double>(successCount) / experiments;

    double theoreticalProbability = 8.0 / 27.0;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Количество экспериментов: " << experiments << "\n\n";
    std::cout << "Эмпирическая вероятность: " << empiricalProbability << "\n";
    std::cout << "Теоретическая вероятность: " << theoreticalProbability << "\n";

    return 0;
}