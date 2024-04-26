#pragma once 

#include "../alg.h"

enum Extremum {
    MIN,
    MAX
};

template <std::size_t T, std::size_t MatrixLines, typename CountType>
std::tuple<std::vector<std::array<CountType, MatrixLines + 1>>, std::array<CountType, MatrixLines + 1>, Extremum> 
getDualProblem(std::vector<std::array<CountType, T>> sourceSystem, std::array<CountType, T> sourceFunc, Extremum extr) {
    std::vector<std::array<CountType, MatrixLines + 1>> resMatrix;
    for (int i = 0; i < T; i++) {
        resMatrix.push_back({});
    }
    std::array<CountType, MatrixLines + 1> resFunc;
    resFunc.back() = sourceFunc.back();

    for (int i = 0; i < MatrixLines; i++) {
        resFunc[i] = sourceSystem[i].back();

        for (int j = 0; j < T - 1; j++) {
            resMatrix[j][i] = sourceSystem[i][j];
        }
    }

    for (int j = 0; j < T - 1; j++) {
        resMatrix[j].back() = sourceFunc[j];
    }

    return {resMatrix, resFunc, extr == MIN ? MAX : MIN};
} 

template <std::size_t T, std::size_t MatrixLines, typename CountType, std::size_t ExtendedMatrixSize = T + MatrixLines>
std::tuple<std::array<CountType, ExtendedMatrixSize>, std::vector<std::array<CountType, ExtendedMatrixSize>>> introduceNewVariables(std::vector<std::array<CountType, T>> sourceSystem, std::array<CountType, T> sourceFunc) {
std::vector<std::array<CountType, ExtendedMatrixSize>> newSystem;
    for (int i = 0; i < MatrixLines; i++) {
        newSystem.push_back({});
    }
    std::array<CountType, ExtendedMatrixSize> newFunc;
    for (int i = 0; i < MatrixLines; i++) {
        newSystem[i].back() = sourceSystem[i].back();
        int j;
        for (j = 0; j < T - 1; j++) {
            newSystem[i][j] = sourceSystem[i][j];
        }

        newSystem[i][j + i] = {1};
    }
    newFunc.back() = sourceFunc.back();
    for (int i = 0; i < T - 1; i++) {
        newFunc[i] = sourceFunc[i];
    }

    return {newFunc, newSystem};
}

template <std::size_t T, std::size_t MatrixLines, typename CountType, std::size_t ExtendedMatrixSize = T + MatrixLines>
std::tuple<CountType, std::vector<CountType>> solveDualSimplexMethod(std::vector<std::array<CountType, T>> sourceSystem, std::array<CountType, T> sourceFunc, Extremum extr, CountType EPS) {
    // Если экстремум - минимум
    if (extr == MIN) {
        // То получаем двойственную задачу и решаем её той же функцией
        auto reversed = getDualProblem<T, MatrixLines, CountType>(sourceSystem, sourceFunc, extr);
        auto res = solveDualSimplexMethod<MatrixLines + 1, T - 1, CountType>(std::get<0>(reversed), std::get<1>(reversed), std::get<2>(reversed), EPS);
        auto newF = std::get<1>(res);
        std::rotate(newF.begin(), newF.begin() + T - 2, newF.end());

        return {std::get<0>(res), newF};
    }

    // Вводим дополнительные переменные
    auto newVars = introduceNewVariables<T, MatrixLines, CountType>(sourceSystem, sourceFunc);
    auto newFunc = std::get<0>(newVars);
    auto newSystem = std::get<1>(newVars);

    // Решаем обычным симплекс-методом
    CountType ans = solveSimplexMethodMax(newSystem, newFunc, EPS);
    // Возвращаем последнюю строчку
    return {ans, std::vector<CountType>(newFunc.begin(), newFunc.end() - 1)};
}