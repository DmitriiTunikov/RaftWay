import sys
import time
from math import pi
import numpy as np
from scipy.optimize import fsolve

def has_in(results, number):
    for res in results:
        if abs(number - res) < 0.00001:
            return True
    return False


def calculate_angles(a, b, d, k):
    def f(x):
        co = np.cos(x)
        so = np.sin(x)
        return a * (co ** 2) + b * (so ** 2) + d * co + k * so

    pi2 = 2 * pi
    x_range = np.linspace(0, pi2, 100)
    y_range = f(x_range)  # type: float

    pfit = np.polyfit(x_range, y_range, 5)

    roots = np.roots(pfit)

    res_root = []
    for i in roots:
        if i.imag != 0.0 or (i.real < 0.0) or (i.real > pi2):
            continue
        cur_res = fsolve(f, i.real)
        if not (has_in(res_root, cur_res[0])):
            res_root.append(cur_res[0])
    return res_root


res_root = calculate_angles(float(sys.argv[1]), float(sys.argv[2]),
                            float(sys.argv[3]), float(sys.argv[4]))
file = open(str(sys.argv[5]), "w")
file.write(str(len(res_root)) + " ")
for r in res_root:
  file.write(str(r) + " ")
file.close()
