#include <iostream>
#include <iomanip>

#include "libs/alg/lab1/task1.tpp"

int main() {
    /*std::vector<std::vector<double>> matrix = {
    {1,  -4,  8,  9, -3, -1, 87},
    {8,   1, -3,  4,  5,  6, 11},
    {4,   0,  1,  3, -2, -5, 17},
    {-3, -4,  7,  6, -1,  4, 70}};*/

    std::vector<std::vector<double>> matrix = {
    {2, -9, 2, 0, 1, 7},
    {1, -5, 3, 4, 0, 4},
    {1, -4, -1, -4, 1, 3},
    {3, -13, 1, -4, 2, 10}};

    auto res = getBasises(matrix);

    for (auto& matrix : res) {
        for (auto & line : matrix) {
            for (auto & element : line) {
                std::cout << std::setw(15) << element << " ";
            }

            std::cout << "\n";
        }
        
        std::cout << "\n";
    }
}