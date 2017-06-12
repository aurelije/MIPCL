#!/usr/bin/python
from mipcl_py.models.revenue import Revenue

# for each plane type:
#     (f   (q1, tl1,tr1),  (q2, tl2,tr2), (q3, tl3,tr3))
planes = (
  {
     'fixedCost': 75000,
     'classes': (
         {'name': 'first', 'capacity': (30,0,12)},
         {'name': 'busines', 'capacity': (40,12,6)},
         {'name': 'economy', 'capacity': (80,12,0)}
     )
  },
  {
     'fixedCost': 150000,
     'classes': (
        {'name': 'first', 'capacity': (60,0,24)},
        {'name': 'busines', 'capacity': (80,24,12)},
        {'name': 'economy', 'capacity': (160,24,0)}
     )
  },
  {
     'fixedCost': 225000,
     'classes': (
        {'name': 'first', 'capacity': (90,0,36)},
        {'name': 'business', 'capacity': (120,36,18)},
        {'name': 'economy', 'capacity': (240,36,0)}
     )
  }
)

# for each of 3 periods:
#     in each of 3 options:
#         prices of tickets of each of 3 classes
options = [
 ((1500, 1000, 500), (1250,  850, 400), (1000,  700, 300)),
 ((1750, 1250, 700), (1500, 1000, 500), (1250,  850, 400)),
 ((1800,  800, 450), (1200,  850, 500), ( 900,  600, 450))
]

nodes = (
  {'name':  0, 'period': 0, 'parent': -1, 'prob': 1.0},
  {'name':  1, 'period': 1, 'parent': 0,  'prob': 0.1, 'demands': (( 25,  40, 100), ( 30,  50, 120), ( 35,  70, 130))},
  {'name':  2, 'period': 1, 'parent': 0,  'prob': 0.6, 'demands': (( 40,  90, 100), ( 50,  85, 100), ( 70,  90, 140))},
  {'name':  3, 'period': 1, 'parent': 0,  'prob': 0.3, 'demands': (( 90,  90, 110), (100,  95, 105), (120,  95, 135))},
  {'name':  4, 'period': 2, 'parent': 1,  'prob': 0.1, 'demands': (( 40,  85, 100), ( 50,  90, 105), ( 65,  85, 125))},
  {'name':  5, 'period': 2, 'parent': 1,  'prob': 0.6, 'demands': (( 20, 100, 120), ( 80, 120, 130), (100, 160, 180))},
  {'name':  6, 'period': 2, 'parent': 1,  'prob': 0.3, 'demands': ((100,  40,  25), (110,  60,  80), (160, 100, 120))},
  {'name':  7, 'period': 2, 'parent': 2,  'prob': 0.1, 'demands': (( 40,  85, 100), ( 50,  90, 105), ( 65,  85, 125))},
  {'name':  8, 'period': 2, 'parent': 2,  'prob': 0.6, 'demands': (( 20, 100, 120), ( 80, 120, 130), (100, 160, 180))},
  {'name':  9, 'period': 2, 'parent': 2,  'prob': 0.3, 'demands': ((100,  40,  25), (110,  60,  80), (160, 100, 120))},
  {'name': 10, 'period': 2, 'parent': 3,  'prob': 0.1, 'demands': (( 40,  85, 100), ( 50,  90, 105), ( 65,  85, 125))},
  {'name': 11, 'period': 2, 'parent': 3,  'prob': 0.6, 'demands': (( 20, 100, 120), ( 80, 120, 130), (100, 160, 180))},
  {'name': 12, 'period': 2, 'parent': 3,  'prob': 0.3, 'demands': ((100,  40,  25), (110,  60,  80), (160, 100, 120))},
  {'name': 13, 'period': 3, 'parent': 4,  'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 14, 'period': 3, 'parent': 4,  'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 15, 'period': 3, 'parent': 4,  'prob': 0.3, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))},
  {'name': 16, 'period': 3, 'parent': 5,  'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 17, 'period': 3, 'parent': 5,  'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 18, 'period': 3, 'parent': 5,  'prob': 0.3, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))},
  {'name': 19, 'period': 3, 'parent': 6,  'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 20, 'period': 3, 'parent': 6,  'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 21, 'period': 3, 'parent': 6,  'prob': 0.3, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))},
  {'name': 22, 'period': 3, 'parent': 7,  'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 23, 'period': 3, 'parent': 7,  'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 24, 'period': 3, 'parent': 7,  'prob': 0.3, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))},
  {'name': 25, 'period': 3, 'parent': 8,  'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 26, 'period': 3, 'parent': 8,  'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 27, 'period': 3, 'parent': 8,  'prob': 0.3, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))},
  {'name': 28, 'period': 3, 'parent': 9,  'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 29, 'period': 3, 'parent': 9,  'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 30, 'period': 3, 'parent': 9,  'prob': 0.3, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))},
  {'name': 31, 'period': 3, 'parent': 10, 'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 32, 'period': 3, 'parent': 10, 'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 33, 'period': 3, 'parent': 10, 'prob': 0.3, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))},
  {'name': 34, 'period': 3, 'parent': 11, 'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 35, 'period': 3, 'parent': 11, 'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 36, 'period': 3, 'parent': 11, 'prob': 0.3, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))},
  {'name': 37, 'period': 3, 'parent': 12, 'prob': 0.1, 'demands': (( 60,  80, 100), ( 70, 100, 120), ( 80, 110, 160))},
  {'name': 38, 'period': 3, 'parent': 12, 'prob': 0.6, 'demands': (( 60,  20, 100), ( 80,  80, 120), (120,  90, 140))},
  {'name': 39, 'period': 3, 'parent': 12, 'prob': 0.1, 'demands': ((100,  80, 120), (140,  90, 130), (160, 120, 140))}
)


prob = Revenue('test1',planes,options,nodes)
prob.model()
prob.optimize()
prob.printSolution()

