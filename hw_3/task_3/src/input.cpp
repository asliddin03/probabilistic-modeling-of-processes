#include "../include/input.h"
#include <string>
#include <stdexcept>

using std::string;
using std::to_string;
using std::runtime_error;

static int to_int_or_throw(const char* x, const string& name) {
    try {
        size_t idx = 0;
        int v = std::stoi(string(x), &idx);
        if (idx != string(x).size()) throw std::invalid_argument("лишние символы");
        return v;
    } catch (...) {
        throw runtime_error("Ошибка: не удалось прочитать целое число для \"" + name + "\".");
    }
}

Input read_args(int argc, char** argv) {
    if (argc < 4) {
        throw runtime_error("Ошибка: ожидается AM RM n q1..qn t_sb t_pr s1..sn mA mR");
    }

    int pos = 1;
    Input in;

    in.AM = to_int_or_throw(argv[pos++], "AM");
    in.RM = to_int_or_throw(argv[pos++], "RM");
    in.n  = to_int_or_throw(argv[pos++], "n");

    if (in.n <= 0 || in.AM < 0 || in.RM < 0) {
        throw runtime_error("Ошибка: AM, RM >= 0, n > 0.");
    }

    // нужно аргументов после имени: 3 + n + 2 + n + 2 = 7 + 2n
    if (argc < 1 + (7 + 2 * in.n)) {
        throw runtime_error("Ошибка: недостаточно аргументов под n=" + to_string(in.n) +
                            ". Ожидается AM RM n q1..qn t_sb t_pr s1..sn mA mR");
    }

    in.q.assign(in.n + 1, 0);
    for (int i = 1; i <= in.n; ++i)
        in.q[i] = to_int_or_throw(argv[pos++], "q" + to_string(i));

    in.t_sb = to_int_or_throw(argv[pos++], "t_sb");
    in.t_pr = to_int_or_throw(argv[pos++], "t_pr");

    in.s.assign(in.n + 1, 0);
    for (int i = 1; i <= in.n; ++i)
        in.s[i] = to_int_or_throw(argv[pos++], "s" + to_string(i));

    in.mA = to_int_or_throw(argv[pos++], "mA");
    in.mR = to_int_or_throw(argv[pos++], "mR");

    if (in.t_sb <= 0 || in.t_pr <= 0) {
        throw runtime_error("Ошибка: t_sb и t_pr должны быть > 0.");
    }
    if (in.mA < 0 || in.mA > 4 || in.mR < 0 || in.mR > 4) {
        throw runtime_error("Ошибка: число модулей должно быть от 0 до 4.");
    }
    for (int i = 1; i <= in.n; ++i) {
        if (in.q[i] < 0 || in.s[i] < 0) {
            throw runtime_error("Ошибка: q_i и s_i должны быть >= 0.");
        }
    }

    return in;
}