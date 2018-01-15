import numpy as np
import copy
from collections import namedtuple, deque
from dualsimplex import DualSimplex

bounds = namedtuple('bounds', 'left right')


class BranchAndBound:
    def __init__(self):
        self.__solver = DualSimplex()

    def calculate(self, Jb, c, A, dLow, dUp, b):
        self.__Jb = Jb
        self.__c = c
        self.__A = A
        self.__b = b

        r = float('-inf')
        x = [0] * A.shape[0]
        tasks = deque()

        res = self.__solve(dLow, dUp)
        if res is None:
            print('non consistent')
            return

        tasks.append(bounds(dLow, dUp))

        while len(tasks) > 0:
            _bounds = tasks.popleft()
            print(_bounds)
            res = self.__solve(_bounds.left, _bounds.right)
            if res is None:
                continue
            isBetter, record = self.__checkBetterX(res, r)
            if not isBetter:
                continue

            if self.__isVectorOfInt(res):
                x = res
                r = record
                continue
            else:
                b1, b2 = self.__separateBounds(res,_bounds)
                tasks.append(b1)
                tasks.append(b2)


        print('record: ' + str(r))
        print('plan: ' + str(x))

    def __solve(self, dLow, dUp):
        return self.__solver.calculate(copy.deepcopy(self.__Jb), self.__c, self.__A, dLow, dUp, self.__b)

    def __checkBetterX(self, plan, record):
        result = 0
        for i in range(0,len(self.__c)-1):
            result += self.__c[i] * plan[i]
        return record < result, result

    def __separateBounds(self, plan, sides):
        b1 = bounds(copy.deepcopy(sides.left), copy.deepcopy(sides.right))
        b2 = bounds(copy.deppcopy(sides.left), copy.deepcopy(sides.right))
        for idx, value in enumerate(plan):
            #if not isinstance(value, int):
            if not (float(value)).is_integer():
                b1.right[idx] = self.__roundDown(value)
                b2.left[idx] = self.__roundUp(value)
        return b1, b2

    def __isVectorOfInt(self, plan):
        for i in plan:
            if not (float(i)).is_integer():
                return False
        return True

    def __roundUp(self, number):
        from math import ceil
        print('roundUp {0} : {1}'.format(number,ceil(number)))
        return ceil(number)

    def __roundDown(self, number):
        print('roundDown {0} : {1}'.format(number,int(number)))
        return int(number)

def test1():
    Jb = [0,1,4,6,7,8,9]
    c = np.array([2,1,-2,-1,4,-5,5,5,1,2]).T

    A = np.matrix([[1,0,0,12,1,-3,4,-1,2.5,3],
                   [0,1,0,11,12,3,5,3,4,5.1],
                   [0,0,1,1,0,22,-2,1,6.1,7]])

    dLow = [0,0,0,0,0,0,0,0,0,0]
    dUp = [2,4,5,3,4,5,4,4,5,6]

    b = np.array([43.5,107.3,106.3]).T

    method = BranchAndBound()
    method.calculate(Jb,c,A,dLow,dUp,b)

if __name__ == '__main__':
    test1()