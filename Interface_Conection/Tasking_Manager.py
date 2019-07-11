## Interface Manager File


from PyQt5 import QtWidgets

from Main_Window_1 import Ui_MainWindow
from Acquisition_Settings_1 import Ui_Form1
from Acquisition_1 import Ui_Form
from ESP301_1 import Ui_ESP301
from NI6229_1 import Ui_NI6229
from Signal_Process import Ui_Dialog



class Firstwindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(Firstwindow, self).__init__(parent)

        ##To add
        self.setupUi1(self)


class Secondwindow(QtWidgets.QMainWindow, Ui_Form1):
    def __init__(self, parent=None):
        super(Secondwindow, self).__init__(parent)
        self.setupUi2(self)

        ##To add
        self.setupUi1(self)

class Thirdwindow(QtWidgets.QMainWindow, Ui_Form):
    def __init__(self, parent=None):
        super(Thirdwindow, self).__init__(parent)
        self.setupUi(self)

        ##To add
        self.setupUi1(self)

class Fourthwindow(QtWidgets.QMainWindow, Ui_ESP301):
    def __init__(self, parent=None):
        super(Fourthwindow, self).__init__(parent)
        self.setupUi3(self)

        ##To add
        self.setupUi1(self)

class Fifthwindow(QtWidgets.QMainWindow, Ui_NI6229):
    def __init__(self, parent=None):
        super(Fifthwindow, self).__init__(parent)
        self.setupUi4(self)

        ##To add
        self.setupUi1(self)

class Swindow(QtWidgets.QDialog, Ui_Dialog):
    def __init__(self, parent=None):
        super(Swindow, self).__init__(parent)
        self.setupUi5(self)

        ##To add
        self.setupUi1(self)



class Manager:


    ##To add


if __name__ == '__main__':

##To add