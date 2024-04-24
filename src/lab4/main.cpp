#include <iostream>
#include <iomanip>
#include <windows.h>
#include <limits>
#include <algorithm>
#include <array>

#include "../libs/alg/alg.h"

int main() {
    // Подготовить входные данные
    std::vector<std::array<double, 5>> c = {
    {{{16}, {26}, {12}, {24}, {3}}},
    {{{5}, {2}, {19}, {27}, {2}}},
    {{{29}, {23}, {25}, {16}, {8}}},
    {{{2}, {25}, {14}, {15}, {21}}}};
    
    std::array<double, 4> a{{{14}, {14 }, {14}, {14}}};
    std::array<double, 5> b{{{13}, {5}, {13}, {12}, {13}}};

    // Вывести ответ на транспортную задачу
    std::cout << solveTransportTaskPotentials(c, a, b, 0.00000001);
}