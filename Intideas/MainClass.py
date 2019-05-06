# This is the Main Window Manager
# Main direct to Records/New Record



from PyQt5 import QtWidgets

from Main import Ui_MainWindow
from NewRec import Ui_MainWindow1
from Records import Ui_MainWindow2



class Firstwindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(Firstwindow, self).__init__(parent)
        self.setupUi(self)
        self.pushButton.clicked.connect(self.hide)
        self.pushButton_2.clicked.connect(self.hide)


class Secondwindow(QtWidgets.QMainWindow, Ui_MainWindow2):
    def __init__(self, parent=None):
        super(Secondwindow, self).__init__(parent)
        self.setup(self)
        self.pushButton.clicked.connect(self.hide)

class Thirdwindow(QtWidgets.QMainWindow, Ui_MainWindow1):
    def __init__(self, parent=None):
        super(Thirdwindow, self).__init__(parent)
        self.setup(self)
        self.pushButton.clicked.connect(self.hide)





class Manager:

    def __init__(self):
        self.first = Firstwindow()
        self.second = Secondwindow()
        self.third = Thirdwindow()


        if  self.first.pushButton_2.clicked.connect(self.second.show):
            self.second.pushButton.clicked.connect(self.first.show)
            self.first.pushButton.clicked.connect(self.third.show)

        elif  self.first.pushButton.clicked.connect(self.third.show):
            self.third.pushButton.clicked.connect(self.first.show)

        else:
            None









        self.first.show()


if __name__ == '__main__':
    import sys
    app = QtWidgets.QApplication(sys.argv)
    manager = Manager()
    sys.exit(app.exec_())