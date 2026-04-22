#include "stats.h"

void Stats::add(long double x) {
    count++;
    sum += x;
    sumSq += x * x;
}

long double Stats::mean() const {
    if (count == 0) return 0.0L;
    return sum / count;
}

long double Stats::variance() const {
    if (count == 0) return 0.0L;
    long double m = mean();
    return sumSq / count - m * m;
}