import sys
import platform
import struct

if not len(sys.argv):
	sys.argv.append('mw.py')
	sys.path.append('./generated_ui/')
	# sys.path.append('%LocalAppData%/Programs/Python/Python37-32/python37.zip')
	# sys.path.append('%LocalAppData%/Programs/Python/Python37-32/DLLs')
	# sys.path.append('%LocalAppData%/Programs/Python/Python37-32')
	# sys.path.append('%LocalAppData%/Programs/Python/Python37-32/Lib')
	# sys.path.append('%LocalAppData%/Programs/Python/Python37-32/Lib/site-packages')
	

from PyQt5 import QtWidgets, uic
from PyQt5.QtCore import QTimer
import numpy as np
import pyqtgraph as pg
import functools

# The libs are being inited from another version of python inside vs ??? -> where is <Python.h> ?
sys.stderr.write('Python version: {}\n' .format(platform.python_version()))
sys.stderr.write('Arch: {}\n' .format(struct.calcsize("P") * 8))
sys.stderr.write('Executable location: {}\n' .format(sys.executable))
sys.stderr.write('System path: {}\n' .format(sys.path))

xz = 0

class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        #Load the UI Page
        uic.loadUi('H:/Data/C++/DCScan/DCScan/generated_ui/mainWindow.ui', self)
		
        x = np.random.normal(size=1000)
        y = np.random.normal(size=1000)

        self.button_start.clicked.connect(self.onClick)

        self.setup()
        self.plot(x, y)

    def timedEvt(self, ms):
        self.timer = QTimer()
        self.timer.timeout.connect(self.onClick)
        self.timer.start(ms)

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
	main.timedEvt(1 / 30) # 30 tps
	sys.exit(app.exec_())

if __name__ == '__main__':         
    main()