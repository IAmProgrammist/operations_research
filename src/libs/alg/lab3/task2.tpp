#pragma once 

#include <vector>
#include <tuple>

template <std::size_t T, std::size_t MatrixLines>
double solveSystemOfLinearEquationsBigPenalties(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    // Сформируем M, где M = сумма коэффециентов ф-ции по модулю
    double M = 0;
    for (auto& v : function) {
        M += std::abs(v);
    }

    // Получаем расширенную матрицу для решения вспомогательной задачи с заданным M и исходной функцией function
    auto expandedMatrix = getDataForAuxTask<T, MatrixLines>(matrix, function, M);

    // Вызовем симплекс метод на преобразованной матрице
    return solveSimplexMethodMaxRaw(std::get<0>(expandedMatrix), std::get<1>(expandedMatrix));
}