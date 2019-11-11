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
import functools
import inspect

# Embedded operation imports from C++
from DCS_Common import *
import DCS_Time as dcst
import DCS_Data as dcsd

# Data configuration in memory
import variables as vars
import data_callback as dcb

# The libs are being inited from another version of python inside vs ??? -> where is <Python.h> ?
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
		
        x = np.random.normal(size=1000)
        y = np.random.normal(size=1000)

        self.button_start.clicked.connect(self.onClick)

        self.setup()
        self.plot(x, y)

        self.timer = dict()

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
    	pass

    def plot(self, x, y):
        self.plt.plot(x, y, pen=None, symbol='o')  # setting pen=None disables line drawing

    def onClick(self):
        global xz
        x = np.linspace(-np.pi + xz * 0.01, np.pi + xz * 0.01, 1000)
        y = np.sin(x)
        xz += 1
        self.plt.clear()
        self.plot(x, y)

def main():
	app = QtWidgets.QApplication(sys.argv)
	main = MainWindow()
	main.show()

	# Add events
	main.addTimedEvent('onClick', 33.3) # 30 tps

	sys.exit(app.exec_())

if __name__ == '__main__':         
    main()