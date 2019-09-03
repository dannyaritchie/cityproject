import matplotlib.pyplot as plt
import numpy as np
from numpy.polynomial.polynomial import polyfit

x, y, z = np.loadtxt('data/closest.txt', delimiter='\t', unpack=True)
b, m = polyfit(x, y, 1)
plt.plot(x, y, '.')
plt.plot(x, b + m * x, '-')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Interesting Graph\nCheck it out')
plt.legend()
plt.show()
