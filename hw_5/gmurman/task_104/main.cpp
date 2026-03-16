#include <iostream>
#include <iomanip>

int main() {

    double pB1_given_A = 0.6;
    double pB2_given_A = 0.3;

    double pB3_given_A = 1.0 - (pB1_given_A + pB2_given_A);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "P(B1|A) = " << pB1_given_A << "\n";
    std::cout << "P(B2|A) = " << pB2_given_A << "\n";
    std::cout << "P(B3|A) = " << pB3_given_A << "\n";

    return 0;
}