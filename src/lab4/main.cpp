#include <iostream>
#include <iomanip>
#include <windows.h>
#include <limits>
#include <algorithm>
#include <array>

#include "../libs/alg/alg.h"

int main() {
    // Подготовить входные данные
    std::vector<std::array<double, 3>> c = {
    {{{5}, {7}, {5}}},
    {{{2}, {1}, {4}}},
    {{{6}, {3}, {2}}}};
    
    std::array<double, 3> a{{{10}, {20}, {50}}};
    std::array<double, 3> b{{{40}, {30}, {10}}};

    std::cout << solveTransportTaskPotentials(c, a, b, 0.00000001);
}