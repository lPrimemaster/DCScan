import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import sys

sys.argv.append('hello.py')

#mpl.use("TkAgg")

t = np.arange(0.0, 2.0, 0.1)
s = np.sin(2.5*np.pi*t)

plt.plot(t, s)
plt.show()