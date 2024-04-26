#pragma once 

#include "../alg.h"

template <std::size_t T, std::size_t MatrixLines, typename CountType, std::size_t ExtendedMatrixSize = T + MatrixLines>
std::tuple<CountType, std::vector<CountType>> solveCommonDualSimplexMethod(std::vector<std::array<CountType, T>> sourceSystem, std::array<CountType, T> sourceFunc, Extremum extr, CountType EPS) {
    // Если экстремум - минимум
    if (extr == MIN) {
        // То получаем двойственную задачу и решаем её той же функцией
        auto reversed = getDualProblem<T, MatrixLines, CountType>(sourceSystem, sourceFunc, extr);
        auto res = solveCommonDualSimplexMethod<MatrixLines + 1, T - 1, CountType>(std::get<0>(reversed), std::get<1>(reversed), std::get<2>(reversed), EPS);
        auto newF = std::get<1>(res);
        std::rotate(newF.begin(), newF.begin() + T - 2, newF.end());

        return {std::get<0>(res), newF};
    }

    // Вводим дополнительные переменные
    auto newVars = introduceNewVariables<T, MatrixLines, CountType>(sourceSystem, sourceFunc);
    auto newFunc = std::get<0>(newVars);
    auto newSystem = std::get<1>(newVars);

    // Строим симплекс-таблицу, копируя в неё матрицу
    std::vector<std::array<CountType, ExtendedMatrixSize>> simplexMatrix(newSystem);
    
    // Добавляем новую строку - целевую функцию, умножая её коэф. yi на -1
    simplexMatrix.push_back(newFunc);
    for (int i = 0; i < T; i++)
        simplexMatrix.back()[i] *= -1;

    CountType minusOne = {-1};
    CountType zero = EPS;
    // Используем метод последовательного уточнения оценок
    while (true) {
        int minRowIndex = -1;
        for (int i = 0; i < MatrixLines; i++)
            if (simplexMatrix[i].back() < zero && (minRowIndex == -1 || simplexMatrix[i].back() < simplexMatrix[minRowIndex].back()))
                minRowIndex = i;
        
        if (minRowIndex == -1) break;

        int minColumnIndex = -1;
        for (int i = 0; i < ExtendedMatrixSize - 1; i++)
            if (simplexMatrix[minRowIndex][i] < zero && (minColumnIndex == -1 || 
            (minusOne * simplexMatrix.back()[i] / simplexMatrix[minRowIndex][i]) < (minusOne * simplexMatrix.back()[minColumnIndex] / simplexMatrix[minRowIndex][minColumnIndex])))
                minColumnIndex = i;

        if (minColumnIndex == -1) throw std::invalid_argument("No solution"); 

        subtractLineFromOther(simplexMatrix, minRowIndex, minColumnIndex, EPS);
    }

    // Возвращам последнюю строчку
    return {simplexMatrix.back().back(), std::vector<CountType>(simplexMatrix.back().begin(), simplexMatrix.back().end() - 1)};
}