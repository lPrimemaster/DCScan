## Start Acquisition

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(562, 254)
        self.pushButton = QtWidgets.QPushButton(Form)
        self.pushButton.setGeometry(QtCore.QRect(50, 60, 101, 23))
        self.pushButton.setObjectName("pushButton")
        self.pushButton_2 = QtWidgets.QPushButton(Form)
        self.pushButton_2.setGeometry(QtCore.QRect(50, 110, 101, 23))
        self.pushButton_2.setObjectName("pushButton_2")
        self.pushButton_3 = QtWidgets.QPushButton(Form)
        self.pushButton_3.setGeometry(QtCore.QRect(280, 190, 121, 23))
        self.pushButton_3.setObjectName("pushButton_3")

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Acquisition"))
        self.pushButton.setText(_translate("Form", "Start Acquisition"))
        self.pushButton_2.setText(_translate("Form", "Change settings"))
        self.pushButton_3.setText(_translate("Form", "Back to Main Window"))



if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Acquisition_1 = QtWidgets.QMainWindow()
    ui = Ui_Form()
    ui.setupUi(Acquisition_1)
    Acquisition_1.show()
    sys.exit(app.exec_())