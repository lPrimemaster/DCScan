import sys
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

islocal = len(sys.argv)

if not islocal:
	sys.argv.append('rtt_module.py')

# Visuals will be defined in the backend side, for now use this
plt.style.use('ggplot')

# Test it out
sys.stderr.write("Script started at: %s\n" % dcst.systemHRC())
print("Script started at: %s\n" % dcst.systemHRC())

def live_plotter(x_vec, y1_data, line1, identifier = '', pause_time = 0.1):
	if line1 == []:
		plt.style.use('dark_background')
		plt.ion()
		fig = plt.figure(figsize = (13,6))
		ax = fig.add_subplot(111)
		
		line1, = ax.plot(x_vec, y1_data, '-o', alpha = 0.8)
		
		plt.ylabel('Y Axis')
		plt.title('Title: {}'.format(identifier))
		plt.show()

	line1.set_ydata(y1_data)
	if np.min(y1_data) <= line1.axes.get_ylim()[0] or np.max(y1_data) >= line1.axes.get_ylim()[1]:
		plt.ylim([np.min(y1_data) - np.std(y1_data), np.max(y1_data) + np.std(y1_data)])
	
	plt.pause(pause_time)
	
	return line1

size = 100
x_vec = np.linspace(0, 1, size + 1)[0:-1]
y_vec = np.random.randn(len(x_vec))
line1 = []

while True:
	npa = dcsd.lastPacket()
	if len(npa):
		y_vec[-1] = npa[0]
	# print(array)
	line1 = live_plotter(x_vec, y_vec, line1, identifier = 'Last frame millis')
	y_vec = np.append(y_vec[1:], 0.0)
