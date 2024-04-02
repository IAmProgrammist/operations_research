#pragma once

#include <vector>
#include <cstdlib>
#include <stdexcept>

#define EPS 0.00000000001

// Сочетания
template<typename T>
std::vector<std::vector<T>> getCombinations(std::vector<T> &baseSet, std::vector<T> currentSet, size_t minIndex, size_t k, size_t count) {
    std::vector<std::vector<T>> resultCombs;

    // Если количество перестановок равно необходимому, мы достигли искомого множества, возвращаем его
    if (count >= k)
        return {currentSet};

    for (size_t i = minIndex; i <= baseSet.size() - k + count; i++) {
        // Добавляем в текущее множество новый элемент, Ci = x
        std::vector<T> newCurrentSet(currentSet);
        newCurrentSet.push_back(baseSet[i]);

        // Вызываем следующий шаг итерации, сохраняем его результат в общий массив множеств
        std::vector<std::vector<T>> combinations = getCombinations(baseSet, newCurrentSet, i + 1, k, count + 1);
        resultCombs.insert(std::begin(resultCombs), std::begin(combinations), std::end(combinations));
    }

    // Возвращаем массив множеств
    return resultCombs;
}

// Функция-обёртка для рекуррентной функции
template<typename T> std::vector<std::vector<T>> getCombinations(std::vector<T> &baseSet, size_t k) {
    return getCombinations(baseSet, {}, 0, k, 0);
}

// Перестановка
template<typename T>
std::vector<std::vector<T>> getPermutations(std::vector<T> baseSet, std::vector<T> currentSet) {
    std::vector<std::vector<T>> resultPerms;

    // Если элементов в изначальном множестве не осталось, получено искомое множество
    if (baseSet.size() == 0) return {currentSet};

    for (size_t i = 0; i < baseSet.size(); i++) {
        // Удаляем из исходного массива x
        std::vector<T> newBaseSet(baseSet);
        newBaseSet.erase(std::begin(newBaseSet) + i);

        // Добавляем в текущее множество новый элемент
        std::vector<T> newCurrentSet(currentSet);
        newCurrentSet.push_back(baseSet[i]);

        // Выполняем следующий шаг итерации, сохраняем в итоговый массив множеств
        auto permutations = getPermutations(newBaseSet, newCurrentSet);
        resultPerms.insert(std::begin(resultPerms), std::begin(permutations), std::end(permutations));
    }

    return resultPerms;
}

template<typename T>
std::vector<std::vector<T>> getPermutations(std::vector<T> &baseSet) {
    return getPermutations(baseSet, {});
}

template <std::size_t T>
void subtractLineFromOther(std::vector<std::array<double, T>>& origin, int indexLeadingLine, int indexEnablingElement) {
    // Преобразуем ведущую строку таким образом, чтобы разрещающий элемент стал равен 1.
    double originEnablingElement = origin[indexLeadingLine][indexEnablingElement];
    for (int i = 0; i < origin[indexLeadingLine].size(); i++) 
        // Разделим каждый элемент строки по индексу indexLeadingLine
        // на разрешающий элемент
        origin[indexLeadingLine][i] /= originEnablingElement;
    
    // Для всех остальных строк с номером i
    for (int i = 0; i < origin.size(); i++) {
        if (i == indexLeadingLine) continue;

        // k - коэффициент, на который нужно домножить ведущую строку и вычесть её
        double k = origin[i][indexEnablingElement];

        // Для всех остальных элементов j в строке i
        for (int j = 0; j < origin[indexLeadingLine].size(); j++)
            // Вычесть ведущую строку indexLeadingLine из строки i
            origin[i][j] -= origin[indexLeadingLine][j] * k;
    }
}

// Вспомогательная СД
template <std::size_t T>
struct Basis {
    std::vector<int> indices;
    std::vector<std::array<double, T>> matrix;
};

template <std::size_t T>
std::vector<Basis<T>> getAllBasises(std::vector<std::array<double, T>> origin, 
std::vector<int>* filterIndices = nullptr, std::vector<int>* requiredIndices = nullptr) {
    // result - массив полученных систем 
    std::vector<Basis<T>> result;

    // indices - массив неизвестных в системе, заполняем
    // его индексами 0 ... l - 1, где l - длина 
    // матрицы origin
    std::vector<int> indices;
    if (filterIndices == nullptr)
        for (int i = 0; i < origin[0].size() - 1; i++) 
            indices.push_back(i);
    else indices = *filterIndices;
    
    /*
    Для каждого сочетания в indices из l по h (h - высота матрицы) 
    получим набор базисных неизвестных basis.
    */
    for (auto basis : getCombinations(indices, (requiredIndices ? (origin.size() - requiredIndices->size()) : origin.size()))) {
        if (requiredIndices)
            basis.insert(basis.begin(), requiredIndices->begin(), requiredIndices->end());

        // Для каждой перестановки строк в матрице origin matrixPermutation
        for (auto matrixPermutation : getPermutations(origin)) {
            auto copyMatrixPermutation = matrixPermutation;
            bool badPermutation = false;
            
            // Для каждой строки в матрице (ввод счётчика строк i)
            for (int i = 0; i < matrixPermutation.size(); i++) {
                // Разрешающий элемент равен 0? 
                if (std::abs(matrixPermutation[i][basis[i]]) < EPS) {
                    bool allZeros = true;
                    for (int j = 0; (j < matrixPermutation[i].size() - 1) && allZeros; j++) {
                        if (std::abs(matrixPermutation[i][j]) > EPS) {
                            // Неудачное расположение строк, необходимо
                            // перейти к другой перестановке
                            badPermutation = true;
                            allZeros = false;
                            break;
                        }
                    }

                    // Коэффициенты в i строке содержат только 0?
                    if (!allZeros) {
                        // Перейти к следующей перестановке строк
                        break;
                    }

                    // b_i равен 0?
                    if (std::abs(matrixPermutation[i].back()) > EPS) {
                        // Система несовместима. Вернуть пустой массив.
                        return {};
                    }

                    /*
                    Удалить строку i из матрицы, полученной перестановкой строк,
                    и вызвать функцию getAllBasises с полученной матрицей.
                    */
                    if (allZeros) {
                        copyMatrixPermutation.erase(copyMatrixPermutation.begin() + i);
                        return getAllBasises(copyMatrixPermutation);
                    }

                    break;
                }

                // Выберем ведущую переменную из basis[i], преобразуем i строку и вычтем её из остальных
                subtractLineFromOther(matrixPermutation, i, basis[i]);
            }

            if (badPermutation) continue;

            // Полученную матрицу matrixPermutation добавим в result, закончим перебор перестановок
            result.push_back({basis, matrixPermutation});
            break;
        }
    }

    return result;
}