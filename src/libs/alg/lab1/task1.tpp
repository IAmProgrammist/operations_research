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

void subtractLineFromOther(std::vector<std::vector<double>>& origin, int indexLeadingLine, int indexEnablingElement) {
    // Ведущий элемент равен 0 - нужно выбросить исключение
    if (std::abs(origin[indexLeadingLine][indexEnablingElement]) < EPS) throw std::invalid_argument("Enabling element can't be equal 0");

    // Преобразуем ведущую строку таким образом, чтобы разрещающий элемент стал равен 1
    double originEnablingElement = origin[indexLeadingLine][indexEnablingElement];
    for (int i = 0; i < origin[indexLeadingLine].size(); i++) 
        origin[indexLeadingLine][i] /= originEnablingElement;
    
    // Для остальных строк выполним следующее
    for (int i = 0; i < origin.size(); i++) {
        if (i == indexLeadingLine) continue;

        // k - коэффициент, на который нужно домножить ведущую строку и вычесть её
        double k = origin[i][indexEnablingElement];

        for (int j = 0; j < origin[indexLeadingLine].size(); j++)
            origin[i][j] -= origin[indexLeadingLine][j] * k;
    }
}

// Функция получения базисов
std::vector<std::vector<std::vector<double>>> getBasises(std::vector<std::vector<double>> origin) {
    std::vector<std::vector<std::vector<double>>> result;

    // Создадим массив индексов
    std::vector<int> indices;
    for (int i = 0; i < origin[0].size() - 1; i++) 
        indices.push_back(i);
    
    // Для каждого сочетания из базисов
    for (auto basis : getCombinations(indices, origin.size())) {
        // Будем перебирать все возможные перестановки строк в матрице
        for (auto matrixPermutation : getPermutations(origin)) {
            bool badPermutation = false;
            for (int i = 0; i < matrixPermutation.size(); i++) {
                try {
                    subtractLineFromOther(matrixPermutation, i, basis[i]);
                } catch (std::invalid_argument& ex) {
                    // Проверим, что случилось
                    bool allZeros = true;
                    for (int j = 0; (j < matrixPermutation[i].size() - 1) && allZeros; j++) {
                        if (std::abs(matrixPermutation[i][j]) > EPS) {
                            // Неудачное расположение строк, продолжим перебор
                            badPermutation = true;
                            allZeros = false;
                            break;
                        }
                    }

                    if (!allZeros) break;

                    // Есть нулевая строка с b_i, не равным 0. Поэтому пропадаем. 
                    if (std::abs(matrixPermutation[i].back()) > EPS) return {};

                    if (allZeros) {
                        // Удаляем ненужную строку
                        origin.erase(origin.begin() + i);
                        return getBasises(origin);
                    }

                    break;
                }
            }

            if (badPermutation) continue;

            result.push_back(matrixPermutation);
            break;
        }
    }

    return result;
}