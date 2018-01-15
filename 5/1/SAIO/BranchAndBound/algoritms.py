from __future__ import division
from __future__ import unicode_literals
import math
import numpy as np
from numpy.linalg import LinAlgError
from dualsimplex import DualSimplexMethodWithLimitsCalculator, NoSolution, IterationsOverflow

Inf = 0


class BranchAndBoundMethodCalculator(object):

    def __init__(self, A, b, c, d_top, d_bottom):
        self.A = A
        self.b = b
        self.c = c
        self.d_top = d_top
        self.d_bottom = d_bottom
        self.tasks = [DualSimplexMethodWithLimitsCalculator(self.A, self.b, self.c, self.d_top, self.d_bottom)]

    def get_r0(self):
        x = np.matrix(self.d_bottom)
        vector_c = np.matrix(self.c)
        r = vector_c * x.T
        return r[0, 0]

    def get_f(self, x):
        return (self.c * np.matrix(x).T)[0, 0]

    def check_integer_condition(self, x):
        for i, xi in enumerate(x):
            if abs(xi - round(xi)) >= 0.000001:
                return False, i, xi
        return True, None, None

    def calculate(self):
        a = []
        r0 = float('-inf')
        r0_storage = set()
        u0 = 0
        u = np.matrix([1] * len(self.c))
        x_storage = []

        iterations = 0

        while len(self.tasks) > 0:
            # step 1
            iterations += 1
            print('\nIterations = {}'.format(iterations))
            print('R = {}'.format(r0 if r0 is not Inf else '-Inf'))
            print('Tasks = {}'.format(len(self.tasks)))
            r0_storage.add(round(r0, 0) if r0 is not Inf else None)

            simplex = self.tasks.pop()

            has_solution = True
            try:
                x = simplex.calculate()
                print('x = {}'.format(x))
                print('F(x) = {}'.format(simplex.get_f(x)))
            except (NoSolution, IterationsOverflow):
                print('no solution'.format(simplex.get_f(x)))
                has_solution = False

            # step 2
            if not has_solution:
                continue

            if r0 is not None and simplex.get_f(x) <= r0:
                continue

            # step 3
            x_is_integer, j0, xi = self.check_integer_condition(x)
            if x_is_integer and r0 < simplex.get_f(x):
                u = x
                u0 = 1
                r0 = simplex.get_f(x)
                a.append(r0)
                continue

            if x in x_storage:
                continue
            x_storage.append(x)

            # step 4
            l_j0 = math.ceil(xi) - 1

            d_top = [di if i != j0 else l_j0 for i, di in enumerate(simplex.d_top)]
            d_bottom = [di if i != j0 else l_j0 + 1 for i, di in enumerate(simplex.d_bottom)]

            self.tasks.append(DualSimplexMethodWithLimitsCalculator(self.A, self.b, self.c, d_top, simplex.d_bottom))
            self.tasks.append(DualSimplexMethodWithLimitsCalculator(self.A, self.b, self.c, simplex.d_top, d_bottom))

        print('\nR0 storage {} = {}'.format(len(r0_storage), sorted(r0_storage)))

        # step 5
        if u0 == 1:
            x = u
        elif u0 == 0:
            print("There is no solution");

        return x
