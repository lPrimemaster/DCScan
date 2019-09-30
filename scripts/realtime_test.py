import sys
import numpy as np
# import pandas as pd
# import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
# import matplotlib.animation as animation

#Imports only for embedded operation
import DCS_Time as dcst

islocal = len(sys.argv)

if not islocal:
	sys.argv.append('rtt_module.py')

# Visuals will be defined in the backend side, for now use this
plt.style.use('ggplot')

def live_plotter(x_vec, y1_data, line1, identifier = '', pause_time = 0.1):
	if line1 == []:
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
	rand_val = np.random.randn(1)
	y_vec[-1] = dcst.systemHRC().sec
	line1 = live_plotter(x_vec, y_vec, line1)
	y_vec = np.append(y_vec[1:], 0.0)
