#pragma once 

#include <optional>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "../alg.h"
#include "../fraction.hpp"

template <std::size_t T, typename CountType>
auto getMatrixForSimplexMethod(std::vector<std::array<CountType, T>>& matrix, std::array<CountType, T>& function, CountType EPS) {
    // Определим, какие переменные могут быть базисными в опорном решении и занесём их в список functionBasisVars
    // (Если yi = 0, то переменная может быть базисной)
    std::vector<int> functionBasisVars;
    for (int i = 0; i < function.size() - 1; i++) 
        if (abs(function[i]) <= EPS) 
            functionBasisVars.push_back(i);
    
    // Получим все базисные решения и для каждого решения basis
    for (auto& basis : getAllBasises(matrix, EPS)) {
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

        // Если список базисных переменных из опорного решения basis входит в список
        // functionBasisVars
        std::sort(basis.indices.begin(), basis.indices.end());
        if (std::includes(basis.indices.begin(), basis.indices.end(), functionBasisVars.begin(), functionBasisVars.end()))
            // Возвращаем искомый базис
            return basis;
    }

    // Возвращаем ошибку - получить подходящее преобразование матрицы невозможно
    throw std::invalid_argument("No basis for function found");
}

template <std::size_t T, typename CountType>
CountType solveSimplexMethodMaxRaw(std::vector<std::array<CountType, T>>& matrix, std::array<CountType, T>& function, CountType EPS, std::vector<int>* baseIndices = nullptr) {
    
    // Строим симплекс-таблицу, копируя в неё матрицу matrix
    std::vector<std::array<CountType, T>> simplexMatrix(matrix);
    
    // Добавляем новую строку - целевую функцию, умножая её коэф. yi на -1
    simplexMatrix.push_back(function);
    for (int i = 0; i < T; i++)
        simplexMatrix.back()[i] *= -1;

    // Бесконечный цикл
    while (true) {
        // Найдём наибольший по модулю отрицательный элемент в последней строке, кроме свободного члена.
        int minColumnIndex = -1;
        for (int i = 0; i < T - 1; i++) {
            if (simplexMatrix.back()[i] < CountType() && (minColumnIndex == -1 || simplexMatrix.back()[minColumnIndex] > simplexMatrix.back()[i]))
                minColumnIndex = i;
        }

        // Такой элемент найден?
        if (minColumnIndex == -1) { 
            // Решение получено, можно выходить из цикла
            break;
        }

        // Определим генеральный элемент таблицы
        int minRowIndex = -1;
        for (int i = 0; i < simplexMatrix.size() - 1; i++) {
            if (simplexMatrix[i][minColumnIndex] <= EPS) continue;
            if (minRowIndex == -1) minRowIndex = i;
            else if (simplexMatrix[minRowIndex].back() / simplexMatrix[minRowIndex][minColumnIndex] > 
            simplexMatrix[i].back() / simplexMatrix[i][minColumnIndex]) minRowIndex = i;
        }

        // Такой элемент найден?
        if (minRowIndex == -1) {
            // Решение получить невозможно, возвращаем ошибку
            throw std::invalid_argument("No solution");
        }

        if (baseIndices) 
            (*baseIndices)[minRowIndex] = minColumnIndex;

        // Преобразуем матрицу к новому базисному виду
        subtractLineFromOther(simplexMatrix, minRowIndex, minColumnIndex, EPS);
    }

    for (int i = 0; i < matrix.size(); i++) 
        for (int j = 0; j < matrix[i].size(); j++) 
            matrix[i][j] = simplexMatrix[i][j];
    
    for (int i = 0; i < T; i++) 
        function[i] = simplexMatrix.back()[i];

    // Возвращаем свободный член в последней строке
    return simplexMatrix.back().back();
}

template <std::size_t T, typename CountType>
CountType solveSimplexMethodMax(std::vector<std::array<CountType, T>>& matrix, std::array<CountType, T>& function, CountType EPS) {
    // Подготовим матрицу таким образом, чтобы в целевой функции были использованы только свободные переменные. 
    // Также отберём опорное решение, в котором все свободные члены больше или равны нулю
    auto preparedMatrix = getMatrixForSimplexMethod(matrix, function, EPS);
    
    // Вызовем симплекс метод на преобразованной матрице
    return solveSimplexMethodMaxRaw(preparedMatrix.matrix, function, EPS);
}