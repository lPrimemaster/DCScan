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

# Control Module
import backend_handler as bh

sys.stderr.write('Script started at: {}\n' .format(dcst.systemHRC()))
sys.stderr.write('Python version: {}\n' .format(platform.python_version()))
sys.stderr.write('Architecture: {}-bit\n' .format(struct.calcsize("P") * 8))
sys.stderr.write('Executable location: {}\n' .format(sys.executable))
sys.stderr.write('System path: {}\n' .format(sys.path))

# Register C++ callbacks
dcsd.registerDataCallback('data_callback', 'data_callback')

class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
    	
        super(MainWindow, self).__init__(*args, **kwargs)

        # Load the UI Page
        uic.loadUi('generated_ui/mainWindow.ui', self)

        # Setup timers
        self.timer = dict()

        # Setup backend handler
        self.serverHandler = bh.BHandler()

        # Setup the rest
        self.setup()

    def addButtonCallback(self, button, callback):
    	getattr(self, button).clicked.connect(getattr(self, callback))

    def addButtonCallbackAnonymous(self, button, callback):
    	getattr(self, button).clicked.connect(callback)

    def addTimedEvent(self, name, ms=1000):
        self.timer[name] = QTimer()
        self.timer[name].timeout.connect(getattr(self, name))
        # self.timer[name].start(ms) -- DONT START BY DEFAULT!

    def startTimedEvent(self, name, ms=1000):
    	self.timer[name].start()

    def stopTimedEvent(self, name):
    	self.timer[name].stop()

    def setup(self):
		# Setup graph
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

        # Setup Async Events
        self.addTimedEvent('getData', 50) # 20 tps

        # Setup buttons
        self.addButtonCallbackAnonymous('button_start', lambda: (self.startTimedEvent('getData', 50), self.serverHandler.startServerAcquisition()))
        self.addButtonCallbackAnonymous('button_pause', lambda: (self.stopTimedEvent('getData')))
        self.addButtonCallbackAnonymous('button_reset', lambda: (self.stopTimedEvent('getData'), self.serverHandler.discardCounts(), self.plt.clear()))
        self.addButtonCallbackAnonymous('button_stop' , lambda: (self.stopTimedEvent('getData'), self.serverHandler.stopServerAcquisition()))
        self.addButtonCallbackAnonymous('button_exit' , lambda: (self.serverHandler.signalTerminate(), QtWidgets.QApplication.quit()))

        # Setup other widgets
        self.led_PXI.turn_on()
        self.led_PXI.setFixedSize(20, 20)

    def getData(self):
        cbd_list = list(vars.DReserved_totals.values())
        list_size = len(cbd_list) # This is the angle entry in dict (normalized)
        cbd_first_key = 0
        if list_size != 0:
            cbd_first_key = list(vars.DReserved_totals.keys())[0]

        x = np.arange(cbd_first_key, list_size + cbd_first_key)
        count = [i[0] for i in cbd_list]
        self.plt.clear()
        self.plot(x, count)

    def plot(self, x, y):
        self.plt.plot(x, y, pen=None, symbol='o')  # setting pen=None disables line drawing

def main():
	dcsd.registerDataCallback('data_callback', 'data_callback')

	app = QtWidgets.QApplication(sys.argv)
	main = MainWindow()
	main.show()
	code = app.exec_()
	QtWidgets.QApplication.quit()

if __name__ == '__main__':         
    main()