#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::mt19937;
using std::uniform_int_distribution;

struct InputData {
    int M = 0;
    int n = 0;
    int r = 0;
    vector<char> A;
    int k = 0;
    string P;
    int K = 0;
    int D = 0;
};

struct Results {
    int cnt_all_conn = 0;
    int cnt_exact_D = 0;
    int cnt_all_not = 0;

    bool got_all_conn = false;
    bool got_exact_D = false;
    bool got_all_not = false;

    vector<string> ex_all_conn;
    vector<string> ex_exact_D;
    vector<string> ex_all_not;
};

static bool connected_by_pattern(const string& L, const string& R, const string& P) {
    int n = (int)L.size();
    int k = (int)P.size();

    for (int t = 1; t <= k - 1; ++t) {
        if (t <= n && (k - t) <= n) {
            string s = L.substr(n - t, t) + R.substr(0, k - t);
            if (s == P) return true;
        }
    }
    return false;
}

static string gen_cluster(int n, const vector<char>& alphabet, mt19937& rng) {
    uniform_int_distribution<int> dist(0, (int)alphabet.size() - 1);
    string s;
    s.reserve(n);
    for (int i = 0; i < n; ++i) s.push_back(alphabet[dist(rng)]);
    return s;
}

static bool validate_input(const InputData& in) {
    if ((int)in.P.size() != in.k) {
        cout << "Ошибка: длина паттерна не равна k.\n";
        return false;
    }
    if (in.M < 2) {
        cout << "Ошибка: M должно быть >= 2.\n";
        return false;
    }
    if (in.n <= 0) {
        cout << "Ошибка: n должно быть > 0.\n";
        return false;
    }
    if (in.r <= 0 || (int)in.A.size() != in.r) {
        cout << "Ошибка: некорректный алфавит.\n";
        return false;
    }
    if (in.k <= 1) {
        cout << "Ошибка: k должно быть >= 2.\n";
        return false;
    }
    if (in.K <= 0) {
        cout << "Ошибка: K должно быть > 0.\n";
        return false;
    }
    if (in.D < 0 || in.D > in.M - 1) {
        cout << "Ошибка: D должно быть от 0 до M-1.\n";
        return false;
    }
    return true;
}

static int count_connected_pairs(const vector<string>& C, const string& P) {
    int M = (int)C.size();
    int X = 0;
    for (int i = 0; i < M - 1; ++i) {
        if (connected_by_pattern(C[i], C[i + 1], P)) ++X;
    }
    return X;
}

static vector<string> generate_series(int M, int n, const vector<char>& A, mt19937& rng) {
    vector<string> C(M);
    for (int i = 0; i < M; ++i) C[i] = gen_cluster(n, A, rng);
    return C;
}

static Results run_simulation(const InputData& in) {
    Results res;
    mt19937 rng(std::random_device{}());

    for (int exp = 0; exp < in.K; ++exp) {
        vector<string> C = generate_series(in.M, in.n, in.A, rng);
        int X = count_connected_pairs(C, in.P);

        if (X == in.M - 1) {
            ++res.cnt_all_conn;
            if (!res.got_all_conn) { res.ex_all_conn = C; res.got_all_conn = true; }
        }
        if (X == in.D) {
            ++res.cnt_exact_D;
            if (!res.got_exact_D) { res.ex_exact_D = C; res.got_exact_D = true; }
        }
        if (X == 0) {
            ++res.cnt_all_not;
            if (!res.got_all_not) { res.ex_all_not = C; res.got_all_not = true; }
        }
    }
    return res;
}

static void print_series(const vector<string>& C) {
    for (int i = 0; i < (int)C.size(); ++i) {
        cout << "Кластер " << (i + 1) << ": " << C[i] << "\n";
    }
}

static void print_results(const InputData& in, const Results& res) {
    cout << std::fixed << std::setprecision(6);

    cout << "\nРезультаты (эмпирические вероятности и количества):\n";
    cout << "a) Все пары соединяемы по паттерну: " << (double)res.cnt_all_conn / in.K
         << " (количество: " << res.cnt_all_conn << " из " << in.K << ")\n";

    cout << "b) Ровно D соединяемых соседних пар: " << (double)res.cnt_exact_D / in.K
         << " (количество: " << res.cnt_exact_D << " из " << in.K << ")\n";

    cout << "c) Все пары не соединяемы по паттерну: " << (double)res.cnt_all_not / in.K
         << " (количество: " << res.cnt_all_not << " из " << in.K << ")\n";

    cout << "\nПример для (a):\n";
    if (res.got_all_conn) print_series(res.ex_all_conn);
    else cout << "Не найдено ни одного примера за K экспериментов.\n";

    cout << "\nПример для (b):\n";
    if (res.got_exact_D) print_series(res.ex_exact_D);
    else cout << "Не найдено ни одного примера за K экспериментов.\n";

    cout << "\nПример для (c):\n";
    if (res.got_all_not) print_series(res.ex_all_not);
    else cout << "Не найдено ни одного примера за K экспериментов.\n";
}

static InputData read_input() {
    InputData in;

    cout << "Введите M (число кластеров): ";
    cin >> in.M;

    cout << "Введите n (длина кластера): ";
    cin >> in.n;

    cout << "Введите r (размер алфавита): ";
    cin >> in.r;

    in.A.resize(in.r);
    cout << "Введите " << in.r << " символ(а/ов) алфавита (через пробел): ";
    for (int i = 0; i < in.r; ++i) cin >> in.A[i];

    cout << "Введите k (длина паттерна): ";
    cin >> in.k;

    cout << "Введите k-паттерн: ";
    cin >> in.P;

    cout << "Введите K (число экспериментов): ";
    cin >> in.K;

    cout << "Введите D (число соединяемых соседних пар): ";
    cin >> in.D;

    return in;
}

int main() {

    InputData in = read_input();
    if (!validate_input(in)) return 0;

    Results res = run_simulation(in);
    print_results(in, res);

    return 0;
}