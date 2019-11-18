import sys
import platform
import struct
import os

if not len(sys.argv):
	sys.argv.append('mw.py')
	sys.path.append('./generated_ui/')
	sys.path.append('./scripts/')
	sys.path.append('./scripts/gui/')
	
# Qt Visuals
from PyQt5 import QtWidgets, uic
from PyQt5.QtCore import QTimer

# Math and data management
import numpy as np
import pandas as pd
import pyqtgraph as pg

# Misc
import pprint as pp
import functools
import inspect

# Embedded operation imports from C++
from DCS_Common import *
import DCS_Time as dcst
import DCS_Data as dcsd

# Data configuration in memory
import variables as vars
import data_callback as dcb

sys.stderr.write('Script started at: {}\n' .format(dcst.systemHRC()))
sys.stderr.write('Python version: {}\n' .format(platform.python_version()))
sys.stderr.write('Architecture: {}-bit\n' .format(struct.calcsize("P") * 8))
sys.stderr.write('Executable location: {}\n' .format(sys.executable))
sys.stderr.write('System path: {}\n' .format(sys.path))

# Register C++ callbacks
dcsd.registerDataCallback('data_callback', 'data_callback')

xz = 0

class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
    	
        super(MainWindow, self).__init__(*args, **kwargs)

        #Load the UI Page
        uic.loadUi('generated_ui/mainWindow.ui', self)

        self.setup()

        self.timer = dict()

    def addButtonCallback(self, button, callback):
    	getattr(self, button).clicked.connect(getattr(self, callback))

    # TODO: func name should differ from dict name
    def addTimedEvent(self, name, ms=1000):
        self.timer[name] = QTimer()
        self.timer[name].timeout.connect(getattr(self, name))
        self.timer[name].start(ms)

    def stopTimedEvent(self, name):
    	self.timer[name].stop()

    def setup(self):
        vw = self.graphWidget.getViewWidget()

        l = pg.GraphicsLayout()
        l.setBorder(pg.mkPen(cosmetic=False, width=4.0, color='r'))
        vw.setCentralItem(l)
        self.plt = l.addPlot(0,0)

        # axis.setTickSpacing([(3, 0), (1, 0), (0.25, 0)])
        bottom = self.plt.getAxis('bottom')
        left = self.plt.getAxis('left')
        bottom.setGrid(100)
        bottom.setTickSpacing(100, 25)
        left.setGrid(100)
        left.setTickSpacing(1, 0.5)

    def getData(self):
    	cbd_list = list(vars.DReserved_totals.values())
    	list_size = len(cbd_list) # This is the angle entry in dict (normalized)
    	x = np.arange(list_size)
    	count = [i[0] for i in cbd_list]
    	# sys.stderr.write(pp.pformat(count))
    	self.plt.clear()
    	self.plot(x, count)

    def plot(self, x, y):
        self.plt.plot(x, y, pen=None, symbol='o')  # setting pen=None disables line drawing

def main():
	app = QtWidgets.QApplication(sys.argv)
	main = MainWindow()
	main.show()

	dcsd.registerDataCallback('data_callback', 'data_callback')

	# Add events
	# main.addButtonCallback('button_start', 'onClick')
	main.addTimedEvent('getData', 100) # 10 tps
	code = app.exec_()
	sys.exit(code)

if __name__ == '__main__':         
    main()