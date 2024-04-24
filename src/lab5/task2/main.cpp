#include <vector>
#include <array>

#include "../../libs/alg/alg.h"

int main() {
    // Подготовить входные данные
    /*std::vector<std::array<Fraction, 3>> matrix;
    matrix.push_back({{{1}, {-2}, {1}}});
    matrix.push_back({{{-2}, {1}, {2}}});
    matrix.push_back({{{3}, {1}, {3}}});
    std::array<Fraction, 3> function{{{-1}, {1}, {0}}};*/
    std::vector<std::array<Fraction, 4>> matrix;
    matrix.push_back({{{1}, {-2}, {3}, {-1}}});
    matrix.push_back({{{-2}, {1}, {1}, {1}}});
    std::array<Fraction, 4> function{{{1}, {2}, {3}, {0}}};

    auto g = solveDualSimplexMethod<4, 2, Fraction>(matrix, function, MIN, Fraction());
    std::cout << std::get<0>(g) << std::endl;
    auto l = std::get<1>(g);
    for (int i = 0; i < l.size(); i++) {
        std::cout << l[i] << " ";
    }
}