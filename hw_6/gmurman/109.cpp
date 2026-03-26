#include <iostream>
#include <random>
#include <iomanip>

int main() {
    // Вероятности отказа ламп
    const double p1 = 0.1;
    const double p2 = 0.2;
    const double p3 = 0.3;
    const double p4 = 0.4;

    // Количество экспериментов
    const long long experiments = 1000000;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution lamp1_fail(p1);
    std::bernoulli_distribution lamp2_fail(p2);
    std::bernoulli_distribution lamp3_fail(p3);
    std::bernoulli_distribution lamp4_fail(p4);

    long long exactly_two_failed = 0;
    long long first_and_second_failed = 0;

    for (long long i = 0; i < experiments; ++i) {
        bool f1 = lamp1_fail(gen);
        bool f2 = lamp2_fail(gen);
        bool f3 = lamp3_fail(gen);
        bool f4 = lamp4_fail(gen);

        int failed_count = static_cast<int>(f1) + static_cast<int>(f2)
                         + static_cast<int>(f3) + static_cast<int>(f4);

        // Событие B: отказали ровно 2 лампы
        if (failed_count == 2) {
            ++exactly_two_failed;

            // Событие A: отказали 1-я и 2-я лампы
            if (f1 && f2 && !f3 && !f4) {
                ++first_and_second_failed;
            }
        }
    }

    // Эмпирическая условная вероятность P(A | B)
    double empirical_probability = 0.0;
    if (exactly_two_failed > 0) {
        empirical_probability =
            static_cast<double>(first_and_second_failed) / exactly_two_failed;
    }

    // Теоретическое значение для сравнения
    double theoretical_probability = 0.0084 / 0.2144; // = 0.0391791...

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Количество экспериментов: " << experiments << "\n";
    std::cout << "Случаев, когда отказали ровно 2 лампы: "
              << exactly_two_failed << "\n";
    std::cout << "Случаев, когда отказали именно 1-я и 2-я лампы: "
              << first_and_second_failed << "\n\n";

    std::cout << "Эмпирическая вероятность P(A|B): "
              << empirical_probability << "\n";
    std::cout << "Теоретическая вероятность P(A|B): "
              << theoretical_probability << "\n";

    return 0;
}