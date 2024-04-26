#include <vector>
#include <array>

#include "../libs/alg/alg.h"

template <std::size_t T, std::size_t MatrixLines>
std::tuple<Fraction, std::vector<Fraction>> solveInInt(std::vector<std::array<Fraction, T>> matrix, std::array<Fraction, T> function);

template <>
std::tuple<Fraction, std::vector<Fraction>> solveInInt<15ULL, 12ULL>(std::vector<std::array<Fraction, 15ULL>> matrix, std::array<Fraction, 15ULL> function) {
    throw std::invalid_argument("Too much!");
}

template <std::size_t T, std::size_t MatrixLines>
std::tuple<Fraction, std::vector<Fraction>> solveInInt(std::vector<std::array<Fraction, T>> matrix, std::array<Fraction, T> function) {
    std::cout << "Simplex" << std::endl;
    // Применяем симплекс метод
    auto res = solveSimplexMethodMaxRaw(matrix, function, Fraction());

    // Ищем нецелые свободные члены с максимальным остатком
    int maxFracIndex = -1;
    std::vector<int> freeIndices;
    for (int i = 0; i < matrix.size(); i++) {
        if (!matrix[i].back().isInt()) {
            if (maxFracIndex == -1) {
                maxFracIndex = i;
                continue;
            }

            auto newDouble = matrix[i].back().getFrac();
            auto oldDouble = matrix[maxFracIndex].back().getFrac();
            if (newDouble > oldDouble) maxFracIndex = i;
        }
    }

    // Определяем свободные переменные
    for (int j = 0; j < T; j++) {
        bool metOne = false;
        bool onlyOnesAndZeros = true;
        for (int i = 0; i < MatrixLines; i++) {
            if (!(matrix[i][j] == Fraction() || matrix[i][j] == Fraction(1))) {
                onlyOnesAndZeros = false;
                break;
            }

            if (matrix[i][j] == Fraction(1)) {
                if (metOne) {
                    onlyOnesAndZeros = false;
                    break;
                } else {
                    metOne = true;
                }
            }
        }

        if (!onlyOnesAndZeros) freeIndices.push_back(j);
    }

    // Если все свободные члены целые
    if (maxFracIndex == -1) {
        // Возвращаем ответ
        std::vector<Fraction> ansB;
        for (int i = 0; i < MatrixLines; i++) {
            ansB.push_back(matrix[i].back());
        }

        return {res, ansB};
    }

    // Формируем сечение
    std::array<Fraction, T + 1> newLine;
    newLine[T] = matrix[maxFracIndex].back().getFrac() * Fraction(-1);
    newLine[T - 1] = Fraction(1);
    for (int i = 0; i < T - 1; i++) {
        if (std::find(freeIndices.begin(), freeIndices.end(), i) != freeIndices.end()) {
            newLine[i] = Fraction(-1) * matrix[maxFracIndex][i].getFrac();
        }
    }

    std::vector<std::array<Fraction, T + 1>> simplexMatrix;
    for (int i = 0; i < matrix.size(); i++) {
        simplexMatrix.push_back({});
        for (int j = 0; j < T - 1; j++) {
            simplexMatrix[i][j] = matrix[i][j];
        }

        simplexMatrix[i][T] = matrix[i].back();
    }
    simplexMatrix.push_back(newLine);
    std::array<Fraction, T + 1> funcLine = {};
    for (int i = 0; i < T - 1; i++) {
        funcLine[i] = function[i];
    }
    funcLine[T] = function.back();
    simplexMatrix.push_back(funcLine);


    std::cout << "Marks" << std::endl;
    // Используем метод последовательного уточнения оценок
    while (true) {
        for (int i = 0; i < simplexMatrix.size(); i++) {
            for (int j = 0; j < T + 1; j++) {
                std::cout << std::setw(10) << simplexMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        int minRowIndex = -1;
        for (int i = 0; i < MatrixLines + 1; i++)
            if (simplexMatrix[i].back() < Fraction() && (minRowIndex == -1 || simplexMatrix[i].back() < simplexMatrix[minRowIndex].back()))
                minRowIndex = i;
        
        if (minRowIndex == -1) break;

        int minColumnIndex = -1;
        for (int i = 0; i < T + 1; i++)
            if (simplexMatrix[minRowIndex][i] < Fraction() && (minColumnIndex == -1 || 
            (Fraction(-1) * simplexMatrix.back()[i] / simplexMatrix[minRowIndex][i]) < (Fraction(-1) * simplexMatrix.back()[minColumnIndex] / simplexMatrix[minRowIndex][minColumnIndex])))
                minColumnIndex = i;

        if (minColumnIndex == -1) throw std::invalid_argument("No solution"); 

        subtractLineFromOther(simplexMatrix, minRowIndex, minColumnIndex, Fraction());
    }

    std::array<Fraction, T + 1> newFunction = {};
    for (int i = 0; i < T + 1; i++) {
        newFunction[i] = simplexMatrix.back()[i] * Fraction(-1);
    }

    simplexMatrix.pop_back();

    // Решаем следующую задачу
    return solveInInt<T + 1, MatrixLines + 1>(simplexMatrix, newFunction);
}

int main() {
    std::vector<std::array<Fraction, 6>> matrix;
    matrix.push_back({{{10}, {3}, {1}, {0}, {0}, {40}}});
    matrix.push_back({{{9}, {-4}, {0}, {1}, {0}, {7}}});
    matrix.push_back({{{-4}, {1}, {0}, {0}, {1}, {14}}});
    std::array<Fraction, 6> function{{{9}, {1}, {0}, {0}, {3}, {0}}};
    Fraction EPS;

    auto res = solveInInt<6, 3>(matrix, function);
    std::cout << std::get<0>(res) << std::endl;
}