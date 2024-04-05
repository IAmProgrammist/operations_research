#pragma once

#include <vector>
#include <algorithm>
#include <tuple>

template <std::size_t T, std::size_t MatrixLines, std::size_t NewMatrixLength = T + MatrixLines>
std::tuple<std::vector<std::array<double, NewMatrixLength>>, std::array<double, NewMatrixLength>, std::vector<int>>
getDataForAuxTask(std::vector<std::array<double, T>> matrix, std::array<double, T> initialFunc = {}, double M = 1) {
    // Инициализируем расширенную матрицу, функцию, массив индексов базовых переменных
    std::vector<std::array<double, NewMatrixLength>> newMatrix(MatrixLines);
    std::array<double, NewMatrixLength> newFunc = {};
    for (int i = 0; i < initialFunc.size() - 1; i++)
        newFunc[i] = initialFunc[i];
    
    newFunc[NewMatrixLength - 1] = initialFunc.back();

    std::vector<int> baseIndices;

    // Для каждой строчки исходной матрицы
    for (int i = 0; i < matrix.size(); i++) {
        // Если свободный коэффициент в матрице < 0, умножаем строку i на -1
        if (matrix[i][T - 1] < 0)
            for (int j = 0; j < T; j++)
                matrix[i][j] *= -1;

        // Копируем из исходной матрицы строку
        // в новую матрицу, формируем 
        // новую функцию, складывая 
        // коэффициенты, умноженные на M
        int j = 0;
        for (; j < matrix[0].size() - 1; j++) {
            newMatrix[i][j] = matrix[i][j];
            newFunc[j] += M * matrix[i][j];
        }
        
        // Добавляем искусственные переменные в новую матрицу
        for (int k = 0; k < matrix.size(); k++) {
            if (k == i) newMatrix[i][j + k] = 1.;
            else newMatrix[i][j + k] = 0;
        }

        // Добавляем в матрицу и функцию свободные переменные
        newMatrix[i][T + MatrixLines - 1] = matrix[i][T - 1];
        newFunc[T + MatrixLines - 1] += M * matrix[i][T - 1];

        // Добавляем в массив базовых индексов базовую переменную
        baseIndices.push_back(T + i - 1);
    }

    // Возвращаем матрицу, преобразованную функцию и базис
    return {newMatrix, newFunc, baseIndices};
}

template <std::size_t T, std::size_t MatrixLines>
double solveSystemOfLinearEquationsArtificialBasis(std::vector<std::array<double, T>>& matrix, std::array<double, T>& function) {
    // Получаем расширенную матрицу для решения вспомогательной задачи
    auto expandedMatrix = getDataForAuxTask<T, MatrixLines>(matrix);
    auto newMatrix = std::get<0>(expandedMatrix);
    auto baseIndices = std::get<2>(expandedMatrix);
    // Решаем полученную вспомогательную задачу симплекс методом
    double ans = solveSimplexMethodMaxRaw(newMatrix, std::get<1>(expandedMatrix), &baseIndices);

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

    // Приводим матрицу к новому базису, учитывая filterIndices и requiredIndices
    auto b = getAllBasises(matrix, &filterIndices, &requiredIndices);

    // Если к таковому привести невозможно, выбрасываем ошибку, матрицу привести к нужному виду 
    // не получится
    if (b.empty()) 
        throw std::invalid_argument("No basis found");

    // Вызовем симплекс метод на преобразованной матрице
    return solveSimplexMethodMaxRaw(b[0].matrix, function);
}