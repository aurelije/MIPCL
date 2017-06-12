#!/usr/bin/python
from mipcl_py.models.matrixgame import MatrixGame

A = [
  [0, 2,  5],
  [2, 3,  1],
  [4, 1, -1],
  [3, -2, 1]
]
A1 = [
  [ 3, -2,  2],
  [-1,  4, -2],
  [ 1,  0,  1],
  [ 2, -1,  3]
]

A2= [
  [ 1, 2, 0, 1, -1],
  [-1,  1, 1, 0,  1],
  [ 2,  0, 1, 2,  1]
]

A3= [
  [ 2, 2, 0, 1, -1],
  [-1,  4, 1, 0,  1],
  [ 2,  -4, 1, 2,  1]
]


X = [
  [0, -1, 2],
  [1, 1, -12]
]

prob = MatrixGame("MatrGame")
prob.solveMatrixGame(A3)
prob.printSolution()

