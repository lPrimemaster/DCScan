## Device Setting Selection

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Form1(object):
    def setupUi2(self, Form):
        Form.setObjectName("Form")
        Form.setEnabled(True)
        Form.resize(573, 408)
        self.pushButton = QtWidgets.QPushButton(Form)
        self.pushButton.setGeometry(QtCore.QRect(90, 290, 75, 23))
        self.pushButton.setObjectName("pushButton")
        self.pushButton_2 = QtWidgets.QPushButton(Form)
        self.pushButton_2.setGeometry(QtCore.QRect(380, 290, 75, 23))
        self.pushButton_2.setObjectName("pushButton_2")
        self.label_2 = QtWidgets.QLabel(Form)
        self.label_2.setGeometry(QtCore.QRect(220, 100, 131, 111))
        self.label_2.setObjectName("label_2")

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Aquisition Settings"))
        self.pushButton.setText(_translate("Form", "ESP 301"))
        self.pushButton_2.setText(_translate("Form", "NI6229"))
        self.label_2.setText(_translate("Form", "<html><head/><body><p><span style=\" font-size:12pt;\">Select device</span></p></body></html>"))

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Acquisition_Settings = QtWidgets.QMainWindow()
    ui2 = Ui_Form1()
    ui2.setupUi2(Acquisition_Settings)
    Acquisition_Settings.show()
    sys.exit(app.exec_())