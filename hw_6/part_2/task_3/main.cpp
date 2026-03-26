#include <iostream>
#include <iomanip>

long long comb(int n, int k) {
    if (k < 0 || k > n) {
        return 0;
    }

    if (k > n - k) {
        k = n - k;
    }

    long long result = 1;
    for (int i = 1; i <= k; ++i) {
        result = result * (n - k + i) / i;
    }

    return result;
}

int main() {
    const int totalStudents = 10;

    const int excellentStudents = 3;
    const int goodStudents = 4;
    const int satisfactoryStudents = 2;
    const int poorStudents = 1;

    const int totalQuestions = 20;
    const int questionsInTicket = 3;

    const int excellentKnown = 20;
    const int goodKnown = 16;
    const int satisfactoryKnown = 10;
    const int poorKnown = 5;

    double pH1 = static_cast<double>(excellentStudents) / totalStudents;
    double pH2 = static_cast<double>(goodStudents) / totalStudents;
    double pH3 = static_cast<double>(satisfactoryStudents) / totalStudents;
    double pH4 = static_cast<double>(poorStudents) / totalStudents;

    double pA_H1 = 1.0;
    double pA_H2 = static_cast<double>(comb(goodKnown, questionsInTicket)) /
                   comb(totalQuestions, questionsInTicket);
    double pA_H3 = static_cast<double>(comb(satisfactoryKnown, questionsInTicket)) /
                   comb(totalQuestions, questionsInTicket);
    double pA_H4 = static_cast<double>(comb(poorKnown, questionsInTicket)) /
                   comb(totalQuestions, questionsInTicket);

    double pA = pH1 * pA_H1 + pH2 * pA_H2 + pH3 * pA_H3 + pH4 * pA_H4;

    double pExcellentGivenPass = (pH1 * pA_H1) / pA;
    double pGoodGivenPass      = (pH2 * pA_H2) / pA;
    double pPoorGivenPass      = (pH4 * pA_H4) / pA;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "P(отлично подготовлен | сдал экзамен) = "
              << pExcellentGivenPass << "\n";
    std::cout << "P(хорошо подготовлен | сдал экзамен) = "
              << pGoodGivenPass << "\n";
    std::cout << "P(плохо подготовлен | сдал экзамен) = "
              << pPoorGivenPass << "\n";
    
    return 0;
}