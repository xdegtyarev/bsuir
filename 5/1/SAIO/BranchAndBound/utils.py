from __future__ import print_function
import math
import numpy as np


# Matrix func

def rows_count(matrix):
    return matrix.shape[0]


def cols_count(matrix):
    return matrix.shape[1]


def get_row(matrix, index):
    return matrix[index, :]


def set_row(matrix, index, row):
    matrix[index, :] = row


def get_col(matrix, index):
    return matrix[:, index]


def set_col(matrix, index, col):
    matrix[:, index] = col


def get_submatrix(matrix, cols, rows):
    return matrix[np.ix_(rows, cols)]


def unit_vector(size, index):
    vector = np.zeros(size)
    vector[index] = 1
    return np.matrix(vector)


# Matrix func

def difference(l1, l2):
    l1_s = set(l1)
    l2_s = set(l2)

    return sorted(list( l1_s.difference(l2_s) ))


def join(l1, l2):
    return sorted(l1.extend(l2))


def create_matrix(rows, cols, value=None):
    return [[value] * cols for _ in xrange(0, rows)]


def copy_matrix(matrix):
    rows = len(matrix)
    cols = len(matrix[0])
    new_matrix = create_matrix(rows, cols)

    for i in xrange(0, rows):
        for j in xrange(0, cols):
            new_matrix[i][j] = matrix[i][j]

    return new_matrix


def print_matrix(t, rows=None, cols=None):
    rows = rows or len(t)
    cols = cols or len(t[0])

    for i in xrange(0, rows):
        for j in xrange(0, cols):
            print('{}'.format('-' if t[i][j] is None else t[i][j]), end='\t')
        print()


# Numeric func

def is_integer(x):
    return abs(x - round(x)) < 0.000001


def get_integer(x):
    return math.ceil(x) - 1 if not is_integer(x) else x


# Other

def print_path(edges):
    print(*['{} -> {}'.format(edge.start, edge.end) for edge in edges], sep=', ')
