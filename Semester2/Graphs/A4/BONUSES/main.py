import numpy as np

# Coefficients of the polynomial -x^3 + x^2 + 2x - 1
coefficients = [-1, 1, 2, -1]

# Find the roots using numpy
roots = np.roots(coefficients)
print(roots);