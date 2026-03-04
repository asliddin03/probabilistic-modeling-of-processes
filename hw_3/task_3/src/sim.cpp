#include "../include/sim.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <stdexcept>

using std::vector;
using std::runtime_error;

struct Machine {
    int remaining = 0;
    int level = 0;
};

struct State {
    vector<vector<long long>> comp; // comp[i][L]
    vector<long long> prod;         // prod[L]
    vector<int> times3, times4, times5;
    vector<Machine> A, R;
    int time = 0;
};

static State init_state(const Input& in) {
    State st;
    st.comp.assign(in.n + 1, vector<long long>(6, 0));
    st.prod.assign(6, 0);
    st.A.assign(in.AM, Machine{});
    st.R.assign(in.RM, Machine{});
    st.time = 0;
    return st;
}

static int slowed_time(int t, int m) {
    return (t * (10 + m) + 9) / 10; // ceil
}

static int sample_delta(int baseL, int modules, std::mt19937& rng) {
    int maxUp = 5 - baseL;
    if (modules <= 0 || maxUp <= 0) return 0;

    std::uniform_real_distribution<double> U(0.0, 1.0);
    double u = U(rng);

    double sum = 0.0;
    for (int k = 1; k <= maxUp; ++k) {
        double pk = modules * 52.0 * std::pow(10.0, -2.0 - k);
        sum += pk;
        if (u < sum) return k;
    }
    return 0;
}

static void add_incoming_components(const Input& in, State& st) {
    for (int i = 1; i <= in.n; ++i) st.comp[i][1] += in.s[i];
}

static void finish_assembly(const Input& in, State& st, std::mt19937& rng) {
    for (auto& m : st.A) {
        if (m.remaining <= 0) continue;
        if (--m.remaining != 0) continue;

        int outL = std::min(5, m.level + sample_delta(m.level, in.mA, rng));
        st.prod[outL]++;

        if (outL == 3) st.times3.push_back(st.time);
        if (outL == 4) st.times4.push_back(st.time);
        if (outL == 5) st.times5.push_back(st.time);
    }
}

static void finish_recycle(const Input& in, State& st, std::mt19937& rng) {
    for (auto& m : st.R) {
        if (m.remaining <= 0) continue;
        if (--m.remaining != 0) continue;

        int outL = std::min(5, m.level + sample_delta(m.level, in.mR, rng));

        for (int i = 1; i <= in.n; ++i) {
            long long ret = in.q[i] / 4;
            if (ret) st.comp[i][outL] += ret;
        }
    }
}

static int pick_recycle_level(const State& st) {
    for (int L = 1; L <= 4; ++L) if (st.prod[L] > 0) return L;
    return 0;
}

static void start_recycle(State& st, int tR) {
    for (auto& m : st.R) {
        if (m.remaining > 0) continue;

        int L = pick_recycle_level(st);
        if (!L) return;

        st.prod[L]--;
        m.level = L;
        m.remaining = tR;
    }
}

static bool can_build_L(const Input& in, const State& st, int L) {
    for (int i = 1; i <= in.n; ++i) if (st.comp[i][L] < in.q[i]) return false;
    return true;
}

static int pick_build_level(const Input& in, const State& st) {
    for (int L = 5; L >= 1; --L) if (can_build_L(in, st, L)) return L;
    return 0;
}

static void start_assembly(const Input& in, State& st, int tA) {
    for (auto& m : st.A) {
        if (m.remaining > 0) continue;

        int L = pick_build_level(in, st);
        if (!L) return;

        for (int i = 1; i <= in.n; ++i) st.comp[i][L] -= in.q[i];

        m.level = L;
        m.remaining = tA;
    }
}

static bool all_components_return_from_recycle(const Input& in) {
    for (int i = 1; i <= in.n; ++i) if (in.q[i] / 4 == 0) return false;
    return true;
}

static void validate_reachability_or_throw(const Input& in) {
    if (in.mA == 0 && in.mR == 0) {
        throw runtime_error("Ошибка: mA=0 и mR=0, повышение качества невозможно, легендарные не появятся.");
    }
    if (in.mA == 0 && !all_components_return_from_recycle(in)) {
        throw runtime_error(
            "Ошибка: mA=0 и есть q_i < 4, тогда при переработке 25% округляется в 0.\n"
            "Нельзя получить все компоненты уровней > 1, легендарные недостижимы.\n"
            "Решение: увеличьте все q_i до >= 4 или задайте mA > 0."
        );
    }
}

ResultTimes simulate(const Input& in) {
    validate_reachability_or_throw(in);

    State st = init_state(in);
    std::mt19937 rng(std::random_device{}());

    int tA = slowed_time(in.t_sb, in.mA);
    int tR = slowed_time(in.t_pr, in.mR);

    const int MAX_TIME = 50'000'000;

    while ((int)st.times5.size() < 25 && st.time < MAX_TIME) {
        add_incoming_components(in, st);

        finish_assembly(in, st, rng);
        finish_recycle(in, st, rng);

        if ((int)st.times5.size() >= 25) break;

        start_recycle(st, tR);
        start_assembly(in, st, tA);

        st.time++;
    }

    if ((int)st.times5.size() < 25) {
        throw runtime_error(
            "Моделирование остановлено: достигнут лимит времени, 25 легендарных не получено.\n"
            "Возможные причины: параметры делают процесс слишком медленным или цель недостижима."
        );
    }

    ResultTimes res;
    res.times3 = std::move(st.times3);
    res.times4 = std::move(st.times4);
    res.times5 = std::move(st.times5);
    res.finished_time = st.time;
    return res;
}