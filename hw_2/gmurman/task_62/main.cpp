#include <iostream>
#include <random>
#include <iomanip>

struct Vec {
    double x, y;
};

double cross(const Vec& a, const Vec& b, const Vec& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool point_in_triangle(const Vec& p, const Vec& A, const Vec& B, const Vec& C) {
    // Проверка внутри или на границе через знаки площадей
    double c1 = cross(A, B, p);
    double c2 = cross(B, C, p);
    double c3 = cross(C, A, p);

    bool has_neg = (c1 < 0) || (c2 < 0) || (c3 < 0);
    bool has_pos = (c1 > 0) || (c2 > 0) || (c3 > 0);

    return !(has_neg && has_pos);
}

int main() {
    const double R = 10.0;
    const int trials = 1'000'000;

    // Вершины вписанного правильного треугольника на окружности радиуса R
    const double PI = 3.14159265358979323846;
    Vec A{ R * std::cos(0.0),          R * std::sin(0.0) };
    Vec B{ R * std::cos(2.0*PI/3.0),   R * std::sin(2.0*PI/3.0) };
    Vec C{ R * std::cos(4.0*PI/3.0),   R * std::sin(4.0*PI/3.0) };

    // Генератор случайных точек, равномерных по площади в круге
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist01(0.0, 1.0);
    std::uniform_real_distribution<double> distAng(0.0, 2.0 * PI);

    int count_a = 0;
    int count_b = 0;

    for (int t = 0; t < trials; ++t) {
        int inTri = 0;
        int seg1 = 0, seg2 = 0, seg3 = 0;

        for (int i = 0; i < 4; ++i) {
            // равномерно по площади: radius = R*sqrt(U), theta = Uniform(0,2pi)
            double U = dist01(rng);
            double theta = distAng(rng);
            double radius = R * std::sqrt(U);

            Vec p{ radius * std::cos(theta), radius * std::sin(theta) };

            if (point_in_triangle(p, A, B, C)) {
                inTri++;
            } else {
                if (theta < 2.0 * PI / 3.0) seg1++;
                else if (theta < 4.0 * PI / 3.0) seg2++;
                else seg3++;
            }
        }

        if (inTri == 4) count_a++;

        if (inTri == 1 && seg1 == 1 && seg2 == 1 && seg3 == 1) count_b++;
    }

    double p_a_emp = static_cast<double>(count_a) / trials;
    double p_b_emp = static_cast<double>(count_b) / trials;

    // pT = S(triangle)/S(circle) = (3*sqrt3)/(4*pi)
    double pT = (3.0 * std::sqrt(3.0)) / (4.0 * PI);
    // pS = (1 - pT)/3
    double pS = (1.0 - pT) / 3.0;

    double p_a_th = std::pow(pT, 4);
    double p_b_th = 24.0 * pT * std::pow(pS, 3); // 4 * 3! * pT * pS^3

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Задача 62 (испытаний=" << trials << ", R=" << R << ")\n\n";

    std::cout << "a) Все 4 точки в треугольнике:\n";
    std::cout << "Эмпирика: " << p_a_emp << "\n";
    std::cout << "Теория:   " << p_a_th << "\n\n";

    std::cout << "б) 1 точка в треугольнике и 3 точки по одной в каждый сегмент:\n";
    std::cout << "Эмпирика: " << p_b_emp << "\n";
    std::cout << "Теория:   " << p_b_th << "\n";

    return 0;
}