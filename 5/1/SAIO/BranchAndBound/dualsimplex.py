from __future__ import division
from __future__ import unicode_literals
import numpy as np
from numpy.linalg import LinAlgError
from utils import *

class DualSimplexMethodWithLimitsCalculator(object):

    def __init__(self, A, b, c, d_top, d_bottom):
        self.A = np.matrix(A)
        self.b = np.matrix(b)
        self.c = np.matrix(c)
        self.d_top = d_top
        self.d_bottom = d_bottom
        self.J = range(0, cols_count(self.A));

    def check_args(self):
        assert rows_count(self.A) == cols_count(self.b), 'Invalid args: number of rows A != length vector b'
        assert cols_count(self.A) == cols_count(self.c), 'Invalid args: number of columns A != length vector c'

    def check_basic(self, A_base):
        A_base_det = np.linalg.det(A_base)

        # TODO: A_base_det != 0
        return A_base_det > 0.0  # 'Check basic plan: det A == 0 ({} = {})'.format(A_base_det, 0)

    def get_f(self, x):
        return (self.c * np.matrix(x).T)[0, 0]

    def calculate(self):
        # self.check_args()

        # init
        #J_base = self._get_base_plan()
        N_map_prev = None
        N_map = None
        values = {}

        for J_base in self._get_base_plan():
            j_base_prev1 = None
            j_base_prev2 = None

            try:
                iteration = 0

                n = cols_count(self.A)
                m = rows_count(self.A)

                c_base = get_submatrix(self.c, J_base, (0,))
                A_base = get_submatrix(self.A, J_base, xrange(0, m))
                B = A_base.I

                # 1 step

                y = c_base * B

                delta_j = [(y * self.A[:, j] - self.c[:, j])[0,0] for j in self.J]

                J_plus = []
                J_minus = []

                for j in self.J:
                    if j not in J_base:
                        if delta_j[j] >= 0:
                            J_plus.append(j)
                        else:
                            J_minus.append(j)

                # 2 step
                while True:
                    iteration += 1
                    if iteration > 1000:
                        raise IterationsOverflow

                    # print('Iteration = {}'.format(iteration))

                    N_map_prev = N_map
                    N_map = {j: None for j in self.J}
                    N_map.update({j: self.d_bottom[j] for j in J_plus})
                    N_map.update({j: self.d_top[j] for j in J_minus})

                    # for j in self.J:
                    #     _sum_Aj_N += get_col(self.A, j) * N_map[j]

                    _sum_Aj_N = 0
                    for j in self.J:
                        if j not in J_base:
                            _sum_Aj_N += get_col(self.A, j) * N_map[j]

                    N_base = B * (self.b.T - _sum_Aj_N)

                    i = 0
                    for j in J_base:
                        N_map[j] = N_base[i, 0]
                        i += 1

                    # print('F() = {}'.format(self.get_f([N_map[j] for j in self.J])))
                    # print([N_map[j] for j in self.J])

                    # 3 step
                    j_k = None

                    for i, j in enumerate(J_base):
                        if not (self.d_bottom[j] <= N_map[j] <= self.d_top[j]):
                            j_k = j
                            break
                    else:
                        # print('=================== F() = {}'.format(self.get_f([N_map[j] for j in self.J])))
                        values[self.get_f([N_map[j] for j in self.J])] = [N_map[j] for j in self.J]
                        raise LinAlgError()
                        # return [N_map[j] for j in self.J]

                    # 4 step
                    # j_k found

                    # 5 step
                    if N_map[j_k] < self.d_bottom[j_k]:
                        u_jk = 1
                    else:
                        u_jk = -1

                    delta_y = u_jk * unit_vector(len(J_base), i) * B

                    u_map = {}

                    # for j in J_minus:
                    #     u_map[j] = delta_y * get_col(self.A, j)
                    for j in self.J:
                        u_map[j] = (delta_y * get_col(self.A, j))[0,0]

                    # u_map[j_k] = delta_y * get_col(self.A, j_k)

                    # 6 step
                    sigma_map ={}

                    for j in J_minus:
                        if u_map[j] > 0.0000001:
                            sigma_map[j] = - delta_j[j] / u_map[j]
                        else:
                            sigma_map[j] = None

                    for j in J_plus:
                        if u_map[j] < 0.000000:
                            sigma_map[j] = - delta_j[j] / u_map[j]
                        else:
                            sigma_map[j] = None

                    sigma_0 = None
                    j_star = None
                    for j, value in sigma_map.items():
                        if value is None:
                            continue

                        if j_star is None or sigma_0 is None or value < sigma_0:
                            sigma_0 = value
                            j_star = j
                            continue

                    # 7 step
                    if sigma_0 is None:
                        raise NoSolution()

                    sigma_0 = sigma_0

                    # 8 step

                    # for j in J_plus:
                    #     delta_j[j] += sigma_0 * u_map[j][0, 0]
                    for j in self.J:
                        delta_j[j] += sigma_0 * u_map[j]

                    # delta_j[j_k] += sigma_0 * u_jk

                    for j in J_base:
                        if j != j_k:
                            delta_j[j] = 0.0

                    # 9 step
                    old_j_base = J_base
                    j_base_prev2 = j_base_prev1
                    j_base_prev1 = old_j_base

                    index = J_base.index(j_k)
                    J_base = [j_star if ii == index else e for ii, e in enumerate(J_base)]

                    if old_j_base == J_base:
                        raise LinAlgError()
                    # if j_base_prev2 is not None and j_base_prev2 == J_base:
                    #     raise LinAlgError()

                    # A_base

                    # M = np.identity(m)
                    #
                    # d = B * get_col(self.A, j_star)
                    # d[i, 0] = -1 / d[i, 0]
                    #
                    # set_col(M, i, [e[0, 0] for e in list(d)])

                    A_base = get_submatrix(self.A, J_base, xrange(0, m))
                    B = A_base.I
                    # B = M * B

                    # 10 step

                    if u_jk == 1 and j_star in J_plus:
                        index = J_plus.index(j_star)
                        J_plus = [j_k if ii == index else e for ii, e in enumerate(J_base)]
                    elif u_jk == -1 and j_star in J_plus:
                        J_plus.remove(j_star)
                    elif u_jk == 1 and j_star not in J_plus:
                        J_plus.append(j_k)

                    J_minus = []
                    for j in self.J:
                        if j not in J_plus and j not in J_base:
                            J_minus.append(j)
            except LinAlgError:
                pass

        if values:
            return values[max(values.keys())]
        else:
            if N_map_prev:
                return [N_map_prev[j] for j in self.J]
            else:
                raise NoSolution()
            # raise NoSolution()

    def _get_base_plan(self):
        a_rows = rows_count(self.A)
        a_cols = cols_count(self.A)

        for j in range(0, a_cols - a_rows):
            cols = range(j, j + a_rows)
            _A_base = self.A[:, cols]
            if self.check_basic(_A_base):
                yield cols


class NoSolution(Exception):
    """No solution"""

    def __init__(self):
        super(NoSolution, self).__init__('No solution')


class IterationsOverflow(Exception):
    """Iteration > 10000"""

    def __init__(self):
        super(IterationsOverflow, self).__init__('Iterations overflow')
