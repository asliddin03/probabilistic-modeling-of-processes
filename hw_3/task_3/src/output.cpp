#include "../include/output.h"
#include <iostream>

using std::cout;
using std::vector;

static void print_times_limited(const vector<int>& v, int limit) {
    int n = (int)v.size();
    cout << "Количество: " << n << "\n";
    if (n == 0) return;

    if (n <= 2 * limit) {
        for (int x : v) cout << x << " ";
        cout << "\n";
        return;
    }

    for (int i = 0; i < limit; ++i) cout << v[i] << " ";
    cout << "... ";
    for (int i = n - limit; i < n; ++i) cout << v[i] << " ";
    cout << "\n";
}

void print_result(const ResultTimes& res, int limit) {
    cout << "Времена производства продуктов качества 3:\n";
    print_times_limited(res.times3, limit);
    cout << "\n";

    cout << "Времена производства продуктов качества 4:\n";
    print_times_limited(res.times4, limit);
    cout << "\n";

    cout << "Времена производства продуктов качества 5:\n";
    for (int x : res.times5) cout << x << " ";
    cout << "\n\n";

    cout << "Время окончания моделирования: " << res.finished_time << "\n";
}