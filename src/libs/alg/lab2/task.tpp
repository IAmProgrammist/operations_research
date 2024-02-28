#pragma once 

#include <optional>
#include <vector>
#include <algorithm>

#include "../lab1/task1.tpp"

template <std::size_t T>
auto getMatrixForSimplexMethod(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    std::vector<int> functionBasisVars;
    for (int i = 0; i < function.size() - 1; i++) 
        if (std::abs(function[i]) < EPS) 
            functionBasisVars.push_back(i);
    
    for (auto& basis : getAllBasises(matrix)) {
        bool isAllBsMoreOrEqualToZero = true;
        for (int i = 0; i < basis.matrix.size() && isAllBsMoreOrEqualToZero; i++) {
            if (basis.matrix[i].back() < EPS) 
                isAllBsMoreOrEqualToZero = false;
        }

        // Хоть один из br меньше 0?
        if (!isAllBsMoreOrEqualToZero) { 
            // Перейти к следующему базисному решению
            continue;
        }

        if (std::includes(functionBasisVars.begin(), functionBasisVars.end(), basis.indices.begin(), basis.indices.end()))
            return basis;
    }

    throw std::invalid_argument("No basis for function found");
}

template <std::size_t T>
double solveSimplexMethod(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    // Строим симплекс-таблицу
    std::vector<std::array<double, T>> simplexMatrix(matrix);
    simplexMatrix.push_back(function);
}