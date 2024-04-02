#pragma once

#include <optional>
#include <vector>
#include <algorithm>
#include <iomanip>

template <std::size_t T, std::size_t MatrixLines>
std::vector<std::array<double, T>> getMatrixForSimplexMethodArtificialBasis(std::vector<std::array<double, T>> matrix) {
    constexpr size_t newSize = T + MatrixLines;
    // Функция для вспомогательной задачи
    std::array<double, newSize> yFunction = {};
    
    // Введём новые переменные в матрицу
    std::vector<std::array<double, newSize>> newMatrix(MatrixLines);

    // Индексы базовых переменных
    std::vector<int> baseIndices;

    // Для каждой строчки исходной матрицы
    for (int i = 0; i < matrix.size(); i++) {
        // Если свободный коэффициент в матрице < 0, умножаем строчку на -1
        if (matrix[i][T - 1] < 0)
            for (int j = 0; j < T; j++)
                matrix[i][j] *= -1;

        // Копируем из исходной матрицы значения в матрицу вспомогательной задачи, 
        // формируем вспомогательную функцию, складывая коэффициенты
        int j = 0;
        for (; j < matrix[0].size() - 1; j++) {
            newMatrix[i][j] = matrix[i][j];
            yFunction[j] += matrix[i][j];
        }
        
        // Добавляем искусственные переменные
        for (int k = 0; k < matrix.size(); k++) {
            if (k == i) newMatrix[i][j + k] = 1.;
            else newMatrix[i][j + k] = 0;
        }

        newMatrix[i][T + MatrixLines - 1] = matrix[i][T - 1];
        yFunction[T + MatrixLines - 1] += matrix[i][T - 1];

        baseIndices.push_back(T + i - 1);
    }

    // Решаем полученную вспомогательную задачу симплекс методом
    double ans = solveSimplexMethodMaxRaw(newMatrix, yFunction, &baseIndices);

    // Если ответ для всп. функции не равен нулю - выбрасываем ошибку, матрицу привести к нужному виду 
    // не получится
    if (std::abs(ans) > 0.00000001) 
        throw std::invalid_argument("No basis found");
    
    // Копируем полученную матрицу в исходную, обрезая столбцы с 
    // искусственными коэффициентами
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size() - 1; j++) {
            matrix[i][j] = newMatrix[i][j];
        }

        matrix[i][T - 1] = newMatrix[i].back();
    }

    // Определяем, какие переменные можно внести вместо искусственных переменных. 
    // Переменные в filterIndices могут их заменить, 
    // в requiredIndices - должны остаться в базисе.
    std::vector<int> filterIndices;
    std::vector<int> requiredIndices;
    int cIndex = 0;
    std::sort(baseIndices.begin(), baseIndices.end());
    for (int i = 0; i < T - 1 && cIndex < baseIndices.size(); i++) {
        if (baseIndices[cIndex] > i) {
            filterIndices.push_back(i);
        } else {
            requiredIndices.push_back(i);
            cIndex++;
        }
    }

    // Приводим матрицу к новому базису
    auto b = getAllBasises(matrix, &filterIndices, &requiredIndices);

    // Если к таковому привести невозможно, выбрасываем ошибку, матрицу привести к нужному виду 
    // не получится
    if (b.empty()) 
        throw std::invalid_argument("No basis found");

    // Возвращаем полученную матрицу
    return b[0].matrix;
}

template <std::size_t T, std::size_t MatrixLines>
double solveSystemOfLinearEquationsArtificialBasis(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    // Подготовим матрицу таким образом, чтобы в целевой функции были использованы только свободные переменные. 
    // Также отберём опорное решение, в котором все свободные члены больше или равны нулю
    auto newMatrix = getMatrixForSimplexMethodArtificialBasis<T, MatrixLines>(matrix);

    // Вызовем симплекс метод на преобразованной матрице
    return solveSimplexMethodMaxRaw(newMatrix, function);
}

template <std::size_t T, std::size_t MatrixLines>
double solveSystemOfLinearEquationsBigPenalties(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    // Сформируем M, где M = сумма коэффециентов ф-ции по модулю, умноженные на 3
    double M = 0;
    for (auto& v : function) {
        M += std::abs(v);
    }
    M = 10;

    constexpr size_t newSize = T + MatrixLines;

    // Функция для вспомогательной задачи
    std::array<double, newSize> mFunction = {};
    for (int i = 0; i < function.size() - 1; i++)
        mFunction[i] = function[i];
    
    mFunction[newSize - 1] = function.back();
    
    // Введём новые переменные в матрицу
    std::vector<std::array<double, newSize>> newMatrix(MatrixLines);

    // Индексы базовых переменных
    std::vector<int> baseIndices;

    // Для каждой строчки исходной матрицы
    for (int i = 0; i < matrix.size(); i++) {
        // Если свободный коэффициент в матрице < 0, умножаем строчку на -1
        if (matrix[i][T - 1] < 0)
            for (int j = 0; j < T; j++)
                matrix[i][j] *= -1;

        // Копируем из исходной матрицы значения в матрицу вспомогательной задачи, 
        // формируем вспомогательную функцию, складывая коэффициенты
        int j = 0;
        for (; j < matrix[0].size() - 1; j++) {
            newMatrix[i][j] = matrix[i][j];
            mFunction[j] += M * matrix[i][j];
        }
        
        // Добавляем искусственные переменные
        for (int k = 0; k < matrix.size(); k++) {
            if (k == i) newMatrix[i][j + k] = 1.;
            else newMatrix[i][j + k] = 0;
        }

        newMatrix[i][T + MatrixLines - 1] = matrix[i][T - 1];
        mFunction[T + MatrixLines - 1] += M * matrix[i][T - 1];

        baseIndices.push_back(T + i - 1);
    }

    // Вызовем симплекс метод на преобразованной матрице
    return solveSimplexMethodMaxRaw(newMatrix, mFunction);
}