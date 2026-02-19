#include <iostream>
#include <random>
#include <iomanip>
#include <cmath>

int main() {

    const double R = 10.0;
    const double r = 3.0;
    const long long trials = 1'000'000;

    const double cx = 0.0, cy = 0.0; // центр малого круга

    long long success = 0;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist01(0.0, 1.0);
    std::uniform_real_distribution<double> distAngle(0.0, 2.0 * M_PI);

    for (long long t = 0; t < trials; ++t) {
        double U = dist01(rng);
        double theta = distAngle(rng);

        double radius = R * std::sqrt(U);
        double x = radius * std::cos(theta);
        double y = radius * std::sin(theta);

        double dx = x - cx;
        double dy = y - cy;
        if (dx * dx + dy * dy <= r * r) {
            success++;
        }
    }

    double p = static_cast<double>(success) / trials;
    double p_theory = (r * r) / (R * R);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Результаты моделирования (испытаний=" << trials << ")\n";
    std::cout << "R=" << R << ", r=" << r << "\n";
    std::cout << "Эксперимент: P ~ " << p << "\n";
    std::cout << "Теория:   (r^2)/(R^2) = " << p_theory << "\n";

    return 0;
}
