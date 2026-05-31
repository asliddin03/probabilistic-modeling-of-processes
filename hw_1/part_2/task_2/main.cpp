#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Counts {
    long long total = 0;
    long long ai = 0;
    long long aj = 0;
    long long sum = 0;
    long long mul = 0;
};

void printPermutation(const std::vector<int>& p) {
    for (int x : p)
        std::cout << x << ' ';
    std::cout << '\n';
}

void processPermutation(const std::vector<int>& p, int i, int j, Counts& c) {
    bool Ai = (p[i - 1] == i);
    bool Aj = (p[j - 1] == j);

    c.total++;

    if (Ai) c.ai++;
    if (Aj) c.aj++;

    if (Ai || Aj) {
        c.sum++;
        std::cout << "A" << i << " + A" << j << ": ";
        printPermutation(p);
    }

    if (Ai && Aj) {
        c.mul++;
        std::cout << "A" << i << "A" << j << ":    ";
        printPermutation(p);
    }
}

Counts calculate(int n, int i, int j) {
    Counts c;
    std::vector<int> p(n);

    for (int k = 0; k < n; k++)
        p[k] = k + 1;

    processPermutation(p, i, j, c);

    while (std::next_permutation(p.begin(), p.end())) {
        processPermutation(p, i, j, c);
    }

    return c;
}

int main() {
    int n, i, j;

    std::cout << "Введите N: ";
    std::cin >> n;
    if (n < 2) {
        std::cout << "Некорректное N\n";
        return 0;
    }

    std::cout << "Введите i и j: ";
    std::cin >> i >> j;
    if (i < 1 || i > n || j < 1 || j > n || i == j) {
        std::cout << "Некорректные i и j\n";
        return 0;
    }

    Counts c = calculate(n, i, j);

    double pAi = static_cast<double>(c.ai) / c.total;
    double pAj = static_cast<double>(c.aj) / c.total;
    double pSum = static_cast<double>(c.sum) / c.total;
    double pMul = static_cast<double>(c.mul) / c.total;

    double aAi = 1.0 / n;
    double aAj = 1.0 / n;
    double aMul = 1.0 / (n * (n - 1));
    double aSum = aAi + aAj - aMul;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nВсего перестановок: " << c.total << "\n\n";

    std::cout << "Непосредственный подсчет:\n";
    std::cout << "P(A" << i << ") = " << pAi << '\n';
    std::cout << "P(A" << j << ") = " << pAj << '\n';
    std::cout << "P(A" << i << " + A" << j << ") = " << pSum << '\n';
    std::cout << "P(A" << i << "A" << j << ") = " << pMul << "\n\n";

    std::cout << "Аналитически:\n";
    std::cout << "P(A" << i << ") = " << aAi << '\n';
    std::cout << "P(A" << j << ") = " << aAj << '\n';
    std::cout << "P(A" << i << " + A" << j << ") = " << aSum << '\n';
    std::cout << "P(A" << i << "A" << j << ") = " << aMul << "\n\n";

    return 0;
}