## Interface Manager File


from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtWidgets import QMainWindow, QApplication, QFileDialog
from Main_Window_1 import Ui_MainWindow
from Acquisition_Settings_1 import Ui_Form1
from Acquisition_1 import Ui_Form
from ESP301_1 import Ui_ESP301
from NI6229_1 import Ui_NI6229
from Signal_Process import Ui_Dialog



class Firstwindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(Firstwindow, self).__init__(parent)
        self.setupUi1(self)
        self.pushButton.clicked.connect(self.hide)
        self.pushButton_2.clicked.connect(self.hide)
        self.pushButton_3.clicked.connect(self.hide)


class Secondwindow(QtWidgets.QMainWindow, Ui_Form1):
    def __init__(self, parent=None):
        super(Secondwindow, self).__init__(parent)
        self.setupUi2(self)
        self.pushButton.clicked.connect(self.hide)
        self.pushButton_2.clicked.connect(self.hide)



class Thirdwindow(QtWidgets.QMainWindow, Ui_Form):
    def __init__(self, parent=None):
        super(Thirdwindow, self).__init__(parent)
        self.setupUi(self)
        self.pushButton.clicked.connect(self.hide)
        self.pushButton_2.clicked.connect(self.hide)
        self.pushButton_3.clicked.connect(self.hide)


class Fourthwindow(QtWidgets.QDialog, Ui_ESP301):
    def __init__(self, parent=None):
        super(Fourthwindow, self).__init__(parent)
        self.setupUi3(self)
        self.pushButton_2.clicked.connect(self.hide)


class Fifthwindow(QtWidgets.QMainWindow, Ui_NI6229):
    def __init__(self, parent=None):
        super(Fifthwindow, self).__init__(parent)
        self.setupUi4(self)


class Swindow(QtWidgets.QDialog, Ui_Dialog):
    def __init__(self, parent=None):
        super(Swindow, self).__init__(parent)
        self.setupUi5(self)




class Manager:

    def __init__(self):
        self.first = Firstwindow()
        self.second = Secondwindow()
        self.third = Thirdwindow()
        self.fourth = Fourthwindow()
        self.fifth = Fifthwindow()
        self.swindow = Swindow()

        self.first.pushButton_2.clicked.connect(self.second.show)
        self.first.pushButton_3.clicked.connect(self.third.show)
        self.first.pushButton.clicked.connect(self.first.pick_new)


        self.third.pushButton.clicked.connect(self.swindow.show)
        self.third.pushButton_2.clicked.connect(self.second.show)
        self.third.pushButton_3.clicked.connect(self.first.show)

        self.second.pushButton.clicked.connect(self.fourth.show)
        self.second.pushButton_2.clicked.connect(self.fifth.show)


        self.first.show()

if __name__ == '__main__':
    import sys
    app = QtWidgets.QApplication(sys.argv)
    manager = Manager()
    sys.exit(app.exec_())
