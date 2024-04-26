from scipy.optimize import linprog
import numpy as np
import math as m

A = np.array([[7, 5, 1, 0, 0],
              [4, -6, 0, 3, 0],
              [-3, 4, 0, 0, 1]])
b = np.array([28, 14, 6])
c = np.array([3, 1, 0, 0, - 1])


max_res_simplex_method = None
def method_branch_and_bound(A, b, c, bounds):
    global max_res_simplex_method
    # получаем решение симплекс методом
    res = linprog(-c, A_ub=A, b_ub=b, bounds=bounds, method='highs')
    # если решения нет выходим из рекурсии
    if not res.success:
        return None

    print("Р:", res.x)
    print("Макс ф:", -res.fun)

    # обновляем результат
    if all(is_int(res.x)) and (max_res_simplex_method is None or -res.fun > -max_res_simplex_method.fun):
        max_res_simplex_method = res

    # цикл, если имеются нецелые решения делаем ветку
    for index, x in zip(range(len(res.x)), res.x):
        if not is_int(x):
            bounds_for_bigger_than = bounds.copy()
            bounds_for_bigger_than[index] = (m.ceil(x), None)
            bounds_for_less_than = bounds.copy()
            bounds_for_less_than[index] = (0, m.floor(x))

            method_branch_and_bound(A, b, c, bounds_for_bigger_than)
            method_branch_and_bound(A, b, c, bounds_for_less_than)

    # выводим результат
    return max_res_simplex_method


def is_int(n, epsilon=0.001):
    return abs(n % 1) <= epsilon


if __name__ == '__main__':
    print("Промежуточные решения и целевые функции:")
    res = method_branch_and_bound(A, b, c, [(0, None)]*len(c))
    print()
    print("Решение:", res.x)
    print("Максимизированная функция:", -res.fun)
