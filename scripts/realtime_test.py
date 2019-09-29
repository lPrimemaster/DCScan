import pylab as plt 
from matplotlib.pyplot import figure, show
import numpy as np
import sys

sys.argv.append('rtt.py')

plt.ion()
X = np.linspace(0,4095,16)  
Y = np.linspace(0,10000,16)
f, axarr = plt.subplots(4, sharex=True)
graph_low,  = axarr[0].plot(X,Y,label='SomeLabel')
graph_low.set_ydata(1)

i = 0
while i < 100000:
	print('wait')
	
