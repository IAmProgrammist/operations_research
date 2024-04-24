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

template <std::size_t T, std::size_t MatrixLines, typename CountType>
std::tuple<CountType, std::vector<CountType>> solveDualSimplexMethod(std::vector<std::array<CountType, T>> sourceSystem, std::array<CountType, T> sourceFunc, Extremum extr, CountType EPS) {
    if (extr == MIN) {
        auto reversed = getDualProblem<T, MatrixLines, CountType>(sourceSystem, sourceFunc, extr);
        std::rota
        return solveDualSimplexMethod<MatrixLines + 1, T - 1, CountType>(std::get<0>(reversed), std::get<1>(reversed), std::get<2>(reversed), EPS);
    }

    for (int i = 0; i < MatrixLines; i++) {
        if (sourceSystem[i].back() < EPS) throw std::invalid_argument("Basis is incorrect");
    }

    constexpr size_t ExtendedMatrixSize = T + MatrixLines; 
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

    CountType ans = solveSimplexMethodMaxRaw(newSystem, newFunc, EPS);
    return {ans, std::vector<CountType>(newFunc.begin(), newFunc.end())};
}