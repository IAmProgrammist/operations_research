#include <vector>
#include <array>

#include "../libs/alg/alg.h"

int main() {
    // Подготовить входные данные
    std::vector<std::array<Fraction, 5>> matrix;
    matrix.push_back({{{12}, {6}, {5}, {10}, {1}}});
    matrix.push_back({{{5}, {9}, {7}, {8}, {1}}});
    matrix.push_back({{{9}, {4}, {6}, {8}, {1}}});
    std::array<Fraction, 5> function{{{1}, {1}, {1}, {1}, {0}}};

    // Решить задачу двойственным симплекс методом
    auto ans = solveDualSimplexMethod<5, 3, Fraction>(matrix, function, MAX, Fraction());

    // Вывести ответ
    std::cout << "A = " << std::get<0>(ans) << std::endl;
    std::cout << "Q = ";
    for (int i = 0; i < 4; i++) {
        auto r = (std::get<1>(ans))[i] / std::get<0>(ans);
        std::cout << r << ", ";
    }
    std::cout << std::endl;

    std::cout << "P = ";
    for (int i = 0; i < 3; i++) {
        auto r = (std::get<1>(ans))[4 + i] / std::get<0>(ans);
        std::cout << r << ", ";
    }
}