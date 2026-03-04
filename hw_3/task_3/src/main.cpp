#include <iostream>
#include "../include/input.h"
#include "../include/sim.h"
#include "../include/output.h"

using std::cout;

int main(int argc, char** argv) {
    try {
        Input in = read_args(argc, argv);
        ResultTimes res = simulate(in);
        print_result(res, 50);
        return 0;
    } catch (const std::exception& e) {
        cout << e.what() << "\n";
        return 1;
    }
}