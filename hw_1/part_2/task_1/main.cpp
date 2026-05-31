#include <iostream>
#include <random>
#include <iomanip>

struct Result {
    double firstEven;
    double secondEven;
    double bothEven;
};

bool readInput(int& N, long long& C) {
    std::cout << "Введите N и C: ";

    if (!(std::cin >> N >> C)) {
        std::cout << "Ошибка ввода.\n";
        return false;
    }

    if (N < 2) {
        std::cout << "Ошибка: N должно быть не меньше 2.\n";
        return false;
    }

    if (C <= 0) {
        std::cout << "Ошибка: количество испытаний должно быть положительным.\n";
        return false;
    }

    return true;
}

Result analyticalFixed(int N) {
    int k = N / 2;

    return {
        static_cast<double>(k) / N,
        static_cast<double>(k) / N,
        static_cast<double>(k) / N *
        static_cast<double>(k - 1) / (N - 1)
    };
}

Result analyticalChanged(int N) {
    int k = N / 2;
    int k2 = (N - 1) / 2;

    return {
        static_cast<double>(k) / N,
        static_cast<double>(k2) / (N - 1),
        static_cast<double>(k) / N *
        static_cast<double>(k2) / (N - 1)
    };
}

Result empiricalFixed(int N, long long experiments) {
    std::mt19937 rng(std::random_device{}());

    long long first = 0;
    long long second = 0;
    long long both = 0;

    for (long long i = 0; i < experiments; i++) {
        int x = std::uniform_int_distribution<int>(1, N)(rng);

        int pos = std::uniform_int_distribution<int>(1, N - 1)(rng);

        int y;

        if (pos < x) {
            y = pos;
        } else {
            y = pos + 1;
        }

        bool firstEven = (x % 2 == 0);
        bool secondEven = (y % 2 == 0);

        if (firstEven) first++;
        if (secondEven) second++;
        if (firstEven && secondEven) both++;
    }

    return {
        static_cast<double>(first) / experiments,
        static_cast<double>(second) / experiments,
        static_cast<double>(both) / experiments
    };
}

Result empiricalChanged(int N, long long experiments) {
    std::mt19937 rng(std::random_device{}());

    long long first = 0;
    long long second = 0;
    long long both = 0;

    for (long long i = 0; i < experiments; i++) {
        int x = std::uniform_int_distribution<int>(1, N)(rng);

        int y = std::uniform_int_distribution<int>(1, N - 1)(rng);

        bool firstEven = (x % 2 == 0);
        bool secondEven = (y % 2 == 0);

        if (firstEven) first++;
        if (secondEven) second++;
        if (firstEven && secondEven) both++;
    }

    return {
        static_cast<double>(first) / experiments,
        static_cast<double>(second) / experiments,
        static_cast<double>(both) / experiments
    };
}

void printResults(const std::string& title, const Result& empirical,
                                            const Result& analytical) {
    std::cout << "\n" << title << "\n";

    std::cout << "а) Первый символ четный\n";
    std::cout << "   Эмпирическая:  " << empirical.firstEven << '\n';
    std::cout << "   Аналитическая: " << analytical.firstEven << "\n\n";

    std::cout << "б) Второй символ четный\n";
    std::cout << "   Эмпирическая:  " << empirical.secondEven << '\n';
    std::cout << "   Аналитическая: " << analytical.secondEven << "\n\n";

    std::cout << "в) Оба символа четные\n";
    std::cout << "   Эмпирическая:  " << empirical.bothEven << '\n';
    std::cout << "   Аналитическая: " << analytical.bothEven << "\n";
}

int main() {
    int N;
    long long C;

    if (!readInput(N, C))
        return 0;

    std::cout << std::fixed << std::setprecision(6);

    printResults(
        "Индексы не изменяются",
        empiricalFixed(N, C),
        analyticalFixed(N)
    );

    printResults(
        "Индексы изменяются",
        empiricalChanged(N, C),
        analyticalChanged(N)
    );

    return 0;
}