import sys

if not len(sys.argv):
	sys.argv.append('rtt_module.py')
	sys.path.append('./scripts/')

import numpy as np
# import pandas as pd
# import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
# import matplotlib.animation as animation

# Imports only for embedded operation
from DCS_Common import *
import DCS_Time as dcst
import DCS_Data as dcsd

# Imports all the variables used to save the data on memory during execution
import variables as vars
import data_callback as dcb

# Visuals will be defined in the backend side, for now use this
# plt.style.use('ggplot')

# Test it out
sys.stderr.write("Script started at: %s\n" % dcst.systemHRC())
print("Script started at: %s\n" % dcst.systemHRC())

dcsd.registerDataCallback('data_callback', 'data_callback')

def gaussian(x, mu, sig):
	return np.exp(-np.power(x - mu, 2.) / (2 * np.power(sig, 2.)))

a = [x for x in range(150)]
b = [0 for x in range(150)]

plt.style.use('dark_background')
plt.ion()

(n, bins, patches) = plt.hist(a, 150 - 1, weights = b, facecolor='blue', edgecolor='white', linewidth=1.0, alpha=0.5)
axes = plt.gca()
axes.set_ylim([0, 20])
plt.show()

x = 0
while True:
	print(vars.totals.count[0])
	while x < 149:
		if x in vars.totals.count:
			patches[x].set_height(vars.totals.count[x])
		x += 1
	x = 0
	plt.pause(0.5)