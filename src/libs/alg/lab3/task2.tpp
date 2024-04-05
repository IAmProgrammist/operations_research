#pragma once 

#include <vector>
#include <tuple>

#include "../fraction.hpp"

template <std::size_t T, std::size_t MatrixLines, typename CountType>
CountType solveSystemOfLinearEquationsBigPenalties(std::vector<std::array<CountType, T>>& matrix, std::array<CountType, T>& function, CountType EPS) {
    // Сформируем M, где M = сумма коэффециентов ф-ции по модулю
    CountType M = 0;
    for (auto& v : function) {
        M += abs(v);
    }

    // Получаем расширенную матрицу для решения вспомогательной задачи с заданным M и исходной функцией function
    auto expandedMatrix = getDataForAuxTask<T, MatrixLines>(matrix, EPS, function, M);

    // Вызовем симплекс метод на преобразованной матрице
    return solveSimplexMethodMaxRaw(std::get<0>(expandedMatrix), std::get<1>(expandedMatrix), EPS);
}