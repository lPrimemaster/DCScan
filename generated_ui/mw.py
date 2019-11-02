from PyQt5 import QtWidgets, uic
from PyQt5.QtCore import QTimer
import numpy as np
import pyqtgraph as pg
import sys
import functools

xz = 0

class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        #Load the UI Page
        uic.loadUi('mainwindow.ui', self)
		
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