#pragma once 

#include <vector>
#include <tuple>

#include "../fraction.hpp"

template <std::size_t T, std::size_t MatrixLines, typename CountType>
void getTransportTaskBasisNorthEast(std::vector<std::array<CountType, T>> &c, std::vector<std::array<CountType, T>> &x, std::array<CountType, MatrixLines> &a, std::array<CountType, T> &b, CountType EPS) {
    int k = 0;
    int r = 0;

    // Пока не дошли до края матрицы
    while (k < MatrixLines && r < T) {
        // Если ak == br
        if (abs(a[k] - b[r]) < EPS) {
            if (r == T - 1) {
                // Вычёркиваем строчку
                x[k][r] = b[r];
                a[k] -= b[r];
                r++;
            } else {
                // Вычёркиваем столбец
                x[k][r] = a[k];
                b[r] -= a[k];
                k++;
            }
        // Если ak > br
        } else if (a[k] > b[r]) {
            // Вычёркиваем строчку
            x[k][r] = b[r];
            a[k] -= b[r];
            r++;
        // Если ak < br
        } else {
            // Вычёркиваем столбец
            x[k][r] = a[k];
            b[r] -= a[k];
            k++;
        }
    }
}

template <std::size_t T, std::size_t MatrixLines, typename CountType>
void getTransportTaskBasisLeastCost(std::vector<std::array<CountType, T>> &c, std::vector<std::array<CountType, T>> &x, std::array<CountType, MatrixLines> &a, std::array<CountType, T> &b, CountType EPS) {
    std::array<bool, T> usedCols = {};
    std::array<bool, MatrixLines> usedRows = {};
    while(true) {
        bool foundAny = false;
        int iMin, jMin;

        for (int i = 0; i < MatrixLines; i++) {
            if (usedRows[i]) continue;
            
            for (int j = 0; j < T; j++) {
                if (usedCols[j]) continue;

                if (!foundAny || c[iMin][jMin] > c[i][j]) {
                    iMin = i;
                    jMin = j;

                    foundAny = true;
                }
            }
        }

        if (!foundAny) break;

        if (abs(a[iMin] - b[jMin]) < EPS) {
            if (jMin == T - 1) {
                // Вычёркиваем строчку
                usedCols[jMin] = true;
                x[iMin][jMin] = b[jMin];
                a[iMin] -= b[jMin];
            } else {
                // Вычёркиваем столбец
                usedRows[iMin] = true;
                x[iMin][jMin] = a[iMin];
                b[jMin] -= a[iMin];
            }
        // Если ak > br
        } else if (a[iMin] > b[jMin]) {
            // Вычёркиваем строчку
            usedCols[jMin] = true;
            x[iMin][jMin] = b[jMin];
            a[iMin] -= b[jMin];
        // Если ak < br
        } else {
            // Вычёркиваем столбец
            usedRows[iMin] = true;
            x[iMin][jMin] = a[iMin];
            b[jMin] -= a[iMin];
        }
    }
}

template <std::size_t T, std::size_t MatrixLines, typename CountType>
std::vector<std::tuple<CountType, CountType, std::vector<std::pair<int, int>>>> getTransportTaskCycles(std::vector<std::array<CountType, T>> c, std::vector<std::array<CountType, T>> x, 
std::array<CountType, MatrixLines> a, std::array<CountType, T> b, std::pair<int, int> init, std::vector<std::pair<int, int>> result, std::pair<int, int> curr,
CountType min, bool minAssigned, CountType sum, int it, int state, CountType EPS) {
    std::vector<std::tuple<CountType, CountType, std::vector<std::pair<int, int>>>> recResult;
    std::vector<std::pair<int, int>> selectedNodes;

    if (state == -1 || state == 0) {
        state = 1;
        
        // Ищем вершину среди базисных переменных или изначальную вершину
        for (int k = 0; k < MatrixLines; k++) {
            if (k == curr.first) continue;
            if ((abs(x[k][curr.second]) > EPS && std::find(result.begin(), result.end(), std::pair<int, int>(k, curr.second)) == result.end()) || 
            (it > 1 && std::pair<int, int>(k, curr.second) == init)) {
                selectedNodes.push_back({k, curr.second});
            } 
        }
    } else if (state == -1 || state == 1) {
        state = 0;
        
        for (int k = 0; k < T; k++) {
            if (k == curr.second) continue;
            if ((abs(x[curr.first][k]) > EPS && std::find(result.begin(), result.end(), std::pair<int, int>(curr.first, k)) == result.end()) || 
            (it > 1 && std::pair<int, int>(curr.first, k) == init)) {
                selectedNodes.push_back({curr.first, k});
            } 
        }
    }

    if (selectedNodes.empty()) {
        return {};
    }

    it++;
    for (auto& node : selectedNodes) {
        if (node == init) {
            recResult.push_back({sum, min, result});
            continue;
        }

        auto newMin = min;
        auto newMinAssigned = minAssigned;
        if (it % 2 && (x[node.first][node.second] < newMin || !newMinAssigned)) {
            newMin = x[node.first][node.second];

            newMinAssigned = true;
        }

        auto newSum = sum;
        // Добавляем cij к sum, если вершина со знаком плюс, иначе - минус
        if (it % 2 == 0) {
            newSum += c[node.first][node.second];
        } else {
            newSum -= c[node.first][node.second];
        }
        auto newResult = result;
        newResult.push_back(node);

        auto recNextResult = getTransportTaskCycles(c, x, a, b, init, newResult, node, newMin, newMinAssigned, newSum, it, state, EPS);
        recResult.insert(recResult.end(), recNextResult.begin(), recNextResult.end());
    }

    return recResult;
}

template <std::size_t T, std::size_t MatrixLines, typename CountType>
std::vector<std::tuple<CountType, CountType, std::vector<std::pair<int, int>>>> getTransportTaskCycles(std::vector<std::array<CountType, T>> c, std::vector<std::array<CountType, T>> x, 
std::array<CountType, MatrixLines> a, std::array<CountType, T> b, int i, int j, CountType EPS) {
    // Инициализируем путь result, сумму sum и min
    std::pair<int, int> init = {i, j};
    std::vector<std::pair<int, int>> result = {init};
    std::pair<int, int> curr = init;
    CountType sum = c[i][j];
    bool minAssigned = false;
    CountType min;
    int it = 0;
    int state = -1;

    return getTransportTaskCycles(c, x, a, b, init, result, curr, min, minAssigned, sum, it, state, EPS);
}

template <std::size_t T, typename CountType>
void moveTransportTaskCycle(std::vector<std::array<CountType, T>> &x, std::vector<std::pair<int, int>> path, CountType min, CountType EPS) {
    // Для всех вершин в пути
    for (int i = 0; i < path.size(); i++) {
        // Если вершина с плюсом, добавляем сдвиг min, иначе - вычитаем
        if (i % 2 == 0) {
            x[path[i].first][path[i].second] += min;
        } else {
            x[path[i].first][path[i].second] -= min;
        }
    }
}

template <std::size_t T, std::size_t MatrixLines, typename CountType>
CountType getAnswerTransportTask(std::vector<std::array<CountType, T>> &c, std::vector<std::array<CountType, T>> &x) {
    // Считаем сумму result = cij * xij, для всех i, j 
    CountType result = c[0][0] * x[0][0];
    for (int i = 0; i < MatrixLines; i++) {
        for (int j = 0; j < T; j++) {
            if (i == 0 && j == 0) continue;

            result += c[i][j] * x[i][j];
        }
    }

    // Вернуть result
    return result;
}

template <std::size_t T, std::size_t MatrixLines, typename CountType>
CountType solveTransportTaskDistributionMethod(std::vector<std::array<CountType, T>> c, std::array<CountType, MatrixLines> a, std::array<CountType, T> b, CountType EPS) {
    // Инициализируем x
    std::vector<std::array<CountType, T>> x;
    for (int i = 0; i < MatrixLines; i++) {
        x.push_back({});
    }

    // Приводим систему к опорному решению
    getTransportTaskBasisLeastCost(c, x, a, b, EPS);

    // В бесконечном цикле
    while (true) {
        bool foundAny = false;
        std::tuple<CountType, CountType, std::vector<std::pair<int, int>>> search;

        // Ищем незаполненные клетки, для которых сумма будет отрицательна
        for (int i = 0; i < MatrixLines && !foundAny; i++) {
            for (int j = 0; j < T; j++) {
                if (abs(x[i][j]) > EPS) continue;

                auto resArray = getTransportTaskCycles(c, x, a, b, i, j, EPS);
                if (resArray.empty()) continue;
                auto res = resArray[0];

                if (std::get<0>(res) < -EPS) {
                    search = res;
                    foundAny = true;
                    break;
                }
            }
        }

        if (!foundAny) break;

        std::vector<std::pair<int, int>> path = std::get<2>(search);
        CountType minv = std::get<1>(search);
        auto pukpuk = std::get<0>(search);

        // Если такая сумма есть, то выполняем сдвиг на min, иначе - выходим из цикла
        moveTransportTaskCycle(x, path, minv, EPS);
    }

    // Возвращаем ответ
    return getAnswerTransportTask<T, MatrixLines, CountType>(c, x);
}

template <std::size_t T, std::size_t MatrixLines, typename CountType>
void recalculatePotentials(std::vector<std::array<CountType, T>> &c, std::vector<std::array<CountType, T>> &x, std::array<CountType, T> &potentialsV, std::array<CountType, MatrixLines> &potentialsU, CountType EPS) {
    // Инициализируем просчитанные потенциалы
    std::array<bool, T> potentialsVFound = {};
    std::array<bool, MatrixLines> potentialsUFound = {};

    // Предположим, что u1 = 0
    potentialsUFound[0] = true;
    potentialsU = {0};
    
    // Пока мы находим непросчитанные потенциалы
    bool foundAny = true;
    while (foundAny) {
        foundAny = false;

        // Для всех заполненных клеток, для который ui и vj не вычислено
        for (int i = 0; i < MatrixLines; i++) {
            for (int j = 0; j < T; j++) {
                if (abs(x[i][j]) < EPS) continue;
                if (potentialsUFound[i] && potentialsVFound[j]) continue;

                // Если вычислен ui, вычисляем vj
                if (potentialsUFound[i]) {
                    potentialsVFound[j] = true;
                    potentialsV[j] = c[i][j] - potentialsU[i];

                    foundAny = true;
                }

                // Если вычислен vj, вычисляем ui
                if (potentialsVFound[j]) {
                    potentialsUFound[i] = true;
                    potentialsU[i] = c[i][j] - potentialsV[j];

                    foundAny = true;
                }
            }
        }
    }
    
}

template <std::size_t T, std::size_t MatrixLines, typename CountType>
CountType solveTransportTaskPotentials(std::vector<std::array<CountType, T>> c, std::array<CountType, MatrixLines> a, std::array<CountType, T> b, CountType EPS) {
    // Инициализируем x
    std::vector<std::array<CountType, T>> x;
    for (int i = 0; i < MatrixLines; i++) {
        x.push_back({});
    }

    // Приводим систему к опорному решению
    getTransportTaskBasisLeastCost(c, x, a, b, EPS);

    std::array<CountType, T> potentialsV;
    std::array<CountType, MatrixLines> potentialsU;

    // В бесконечном цикле
    while (true) {
        // Пересчитываем потенциалы
        recalculatePotentials(c, x, potentialsV, potentialsU, EPS);
        int i, j;
        bool foundAny = false;
        // Находим отрицательную сумму
        for (i = 0; i < MatrixLines; i++) {
            for (j = 0; j < T; j++) {
                if (x[i][j] > EPS) continue;

                CountType t = c[i][j] - (potentialsU[i] + potentialsV[j]);

                if (t < -EPS) {
                    foundAny = true;
                    break;
                }
            }
            if (foundAny) break;
        }

        // Если такой суммы нет, выходим из цикла
        if (!foundAny) break;

        // Иначе находим путь для найденной вершины
        auto search = getTransportTaskCycles(c, x, a, b, i, j, EPS)[0];
        auto pukpuk = std::get<0>(search);
        // И выполняем сдвиг на min
        moveTransportTaskCycle(x, std::get<2>(search), std::get<1>(search), EPS);
    }

    // Возвращаем ответ
    return getAnswerTransportTask<T, MatrixLines, CountType>(c, x);
}