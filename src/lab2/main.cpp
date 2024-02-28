#include <iostream>
#include <iomanip>
#include <array>

#include "../libs/alg/alg.h"

int main() {
    std::vector<std::array<double, 7>> matrix = {
    {-1, 3, 0, 6, 0, 1, 26},
    {5, 1, 7, -3, 0, 0, 14},
    {2, -1, 0, -2, 1, 0, 12}};
    std::array<double, 7> function{{6.0, 2.0, 0.0, -5.0, 0.0, 0.0, 0.0}};

    auto lol = getMatrixForSimplexMethod(matrix, function);
    for (auto & row : lol.matrix) {
        for (auto & element : row) {
            std::cout << std::setw(15) << element << " ";
        }

        std::cout << "\n";
    }

    solveSimplexMethod(matrix, function);
}