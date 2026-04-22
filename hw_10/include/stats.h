#pragma once

struct Stats {
    long long count = 0;
    long double sum = 0.0L;
    long double sumSq = 0.0L;

    void add(long double x);
    long double mean() const;
    long double variance() const;
};