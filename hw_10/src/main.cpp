#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <random>

#include "components.h"
#include "cycles.h"
#include "graph.h"
#include "mst.h"
#include "random_graph.h"
#include "stats.h"
#include "trees.h"

int main() {
    int n;
    int experiments;
    double p;

    std::cout << "Введите число вершин n: ";
    std::cin >> n;

    std::cout << "Введите число экспериментов Monte-Carlo: ";
    std::cin >> experiments;

    std::cout << "Введите вероятность существования ребра p (например, 0.5): ";
    std::cin >> p;

    if (n <= 0 || experiments <= 0 || p < 0.0 || p > 1.0) {
        std::cerr << "Некорректные входные данные.\n";
        return 1;
    }

    if (n > 16) {
        std::cout << "\nПредупреждение: для B и D поиск циклов может работать долго при n > 16.\n\n";
    }

    std::mt19937_64 rng(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );

    Stats statA, statB, statD, statE, statG, statH;

    for (int it = 0; it < experiments; ++it) {
        Graph g = generateRandomGraph(n, p, rng);

        int mst = mstWeightKruskal(g);
        if (mst != -1) {
            statA.add(mst);
        }

        CycleInfo ci = analyzeCycles(g);
        statB.add(ci.maxLenCycle);
        statD.add(ci.edgesInMaxWeightCycle);

        statE.add(countIsolatedVertices(g));
        statG.add(countConnectedComponents(g));
        statH.add(countCliqueComponents(g));
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\n===== Результаты =====\n";

    std::cout << "\nA - сумма весов MST (только по связным графам):\n";
    if (statA.count == 0) {
        std::cout << "Связных графов среди сгенерированных не оказалось.\n";
    } else {
        std::cout << "Количество учтенных экспериментов: " << statA.count << "\n";
        std::cout << "Математическое ожидание: " << static_cast<double>(statA.mean()) << "\n";
        std::cout << "Дисперсия: " << static_cast<double>(statA.variance()) << "\n";
    }

    std::cout << "\nB - длина самого большого цикла по числу ребер:\n";
    std::cout << "Математическое ожидание: " << static_cast<double>(statB.mean()) << "\n";
    std::cout << "Дисперсия: " << static_cast<double>(statB.variance()) << "\n";

    std::cout << "\nD - число ребер цикла с максимальной суммой весов:\n";
    std::cout << "Математическое ожидание: " << static_cast<double>(statD.mean()) << "\n";
    std::cout << "Дисперсия: " << static_cast<double>(statD.variance()) << "\n";

    std::cout << "\nE - число изолированных вершин:\n";
    std::cout << "Математическое ожидание: " << static_cast<double>(statE.mean()) << "\n";
    std::cout << "Дисперсия: " << static_cast<double>(statE.variance()) << "\n";

    std::cout << "\nF - число деревьев на n вершинах с точностью до перестановки вершин:\n";
    try {
        long long f = unlabeledTreesCount(n);
        std::cout << "Значение F: " << f << "\n";
        std::cout << "Математическое ожидание: " << f << "\n";
        std::cout << "Дисперсия: 0\n";
    } catch (const std::exception& ex) {
        std::cout << ex.what() << "\n";
    }

    std::cout << "\nG - число компонент связности:\n";
    std::cout << "Математическое ожидание: " << static_cast<double>(statG.mean()) << "\n";
    std::cout << "Дисперсия: " << static_cast<double>(statG.variance()) << "\n";

    std::cout << "\nH - число компонент связности, являющихся полными подграфами:\n";
    std::cout << "Математическое ожидание: " << static_cast<double>(statH.mean()) << "\n";
    std::cout << "Дисперсия: " << static_cast<double>(statH.variance()) << "\n";

    std::cout << "\nПримечание:\n";
    std::cout << "1) Использована модель случайного графа G(n, p).\n";
    std::cout << "2) Вес каждого существующего ребра равновероятно выбирается из [1, 10].\n";
    std::cout << "3) Для A учитываются только связные графы.\n";
    std::cout << "4) Для B и D алгоритм экспоненциальный по n.\n";

    return 0;
}