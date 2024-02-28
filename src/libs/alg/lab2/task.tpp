#pragma once 

#include <optional>
#include <vector>
#include <algorithm>

#include "../lab1/task1.tpp"

template <std::size_t T>
auto getMatrixForSimplexMethod(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    // Определим, какие переменные должны быть базисными в опорном решении и занесём их в список functionBasisVars
    std::vector<int> functionBasisVars;
    for (int i = 0; i < function.size() - 1; i++) 
        if (std::abs(function[i]) < EPS) 
            functionBasisVars.push_back(i);
    
    // Получим все базисные решения
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

        // Если несвободные переменные из функции входят в список базисных из опорного решения
        std::sort(basis.indices.begin(), basis.indices.end());
        if (std::includes(basis.indices.begin(), basis.indices.end(), functionBasisVars.begin(), functionBasisVars.end()))
            // Возвращаем искомый базис
            return basis;
    }

    // Возвращаем ошибку - получить подходящее преобразование матрицы невозможно
    throw std::invalid_argument("No basis for function found");
}

template <std::size_t T>
double solveSimplexMethodMaxRaw(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    auto preparedMatrix = getMatrixForSimplexMethod(matrix, function);
    // Строим симплекс-таблицу, копируя в неё матрицу matrix.
    std::vector<std::array<double, T>> simplexMatrix(preparedMatrix.matrix);
    // Добавляем новую строку - целевую функцию.
    simplexMatrix.push_back(function);
    for (int i = 0; i < T; i++)
        simplexMatrix.back()[i] *= -1;

    while (true) {
        // Найдём наибольший по модулю отрицательный элемент в последней строке, кроме свободного члена.
        int minColumnIndex = -1;
        for (int i = 0; i < T - 1; i++) {
            if (simplexMatrix.back()[i] < 0 && (minColumnIndex == -1 || simplexMatrix.back()[minColumnIndex] > simplexMatrix.back()[i]))
                minColumnIndex = i;
        }

        // Если такого элемента нет, то выходим из цикла.
        if (minColumnIndex == -1) break;

        // Определим генеральный элемент таблицы.
        int minRowIndex = -1;
        for (int i = 0; i < simplexMatrix.size() - 1; i++) {
            if (simplexMatrix[i][minColumnIndex] <= EPS) continue;
            if (minRowIndex == -1) minRowIndex = i;
            else if (simplexMatrix[minRowIndex].back() / simplexMatrix[minRowIndex][minColumnIndex] > 
            simplexMatrix[i].back() / simplexMatrix[i][minColumnIndex]) minRowIndex = i;
        }

        // Если такого элемента нет, то возвращаем ошибку - решения не найдено.
        if (minRowIndex == -1) throw std::invalid_argument("No solution");

        // Преобразуем матрицу к новому базисному виду.
        subtractLineFromOther(simplexMatrix, minRowIndex, minColumnIndex);
    }

    // Возвращаем свободный член в последней строке - ответ.
    return simplexMatrix.back().back();
}

template <std::size_t T>
double solveSimplexMethodMax(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    // Подготовим матрицу таким образом, чтобы в целевой функции были использованы только свободные переменные. 
    // Также отберём опорное решение, в котором все свободные члены больше или равны нулю
    auto preparedMatrix = getMatrixForSimplexMethod(matrix, function);
    
    // Вызовем симплекс метод на преобразованной матрице
    return solveSimplexMethodMaxRaw(preparedMatrix.matrix, function);
}