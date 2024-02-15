#include <iostream>
#include <iomanip>

#include "libs/alg/lab1/task1.tpp"

int main() {
    std::vector<std::vector<double>> matrix = {
    {1,  -4,  8,  9, -3, -1, 87},
    {8,   1, -3,  4,  5,  6, 11},
    {4,   0,  1,  3, -2, -5, 17},
    {-3, -4,  7,  6, -1,  4, 70}};;

    auto res = getAllBasises(matrix);

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