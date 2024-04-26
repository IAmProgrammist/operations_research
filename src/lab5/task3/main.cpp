#include <vector>
#include <array>

#include "../../libs/alg/alg.h"

int main() {
    // Подготовить входные данные
    std::vector<std::array<Fraction, 5>> matrix;
    matrix.push_back({{{-4}, {1}, {-4}, {1}, {15}}});
    matrix.push_back({{{2}, {3}, {4}, {2}, {30}}});
    matrix.push_back({{{3}, {7}, {2}, {-3}, {25}}});
    std::array<Fraction, 5> function{{{-3}, {9}, {1}, {4}, {0}}};

    // Вывод ответа
    auto g = solveCommonDualSimplexMethod<5, 3, Fraction>(matrix, function, MIN, Fraction());
    std::cout << std::get<0>(g) << std::endl;
    auto l = std::get<1>(g);
    for (int i = 0; i < l.size(); i++) {
        std::cout << l[i] << " ";
    }
}