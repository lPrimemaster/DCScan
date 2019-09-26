##NI6229

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_NI6229(object):
    def setupUi4(self, NI6229):
        NI6229.setObjectName("NI6229")
        NI6229.resize(593, 410)
        self.lineEdit = QtWidgets.QLineEdit(NI6229)
        self.lineEdit.setGeometry(QtCore.QRect(50, 70, 111, 20))
        self.lineEdit.setObjectName("lineEdit")
        self.label = QtWidgets.QLabel(NI6229)
        self.label.setGeometry(QtCore.QRect(50, 50, 231, 16))
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(NI6229)
        self.label_2.setGeometry(QtCore.QRect(50, 100, 231, 16))
        self.label_2.setObjectName("label_2")
        self.lineEdit_2 = QtWidgets.QLineEdit(NI6229)
        self.lineEdit_2.setGeometry(QtCore.QRect(50, 120, 111, 20))
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.label_3 = QtWidgets.QLabel(NI6229)
        self.label_3.setGeometry(QtCore.QRect(50, 190, 231, 16))
        self.label_3.setObjectName("label_3")
        self.comboBox = QtWidgets.QComboBox(NI6229)
        self.comboBox.setGeometry(QtCore.QRect(50, 220, 69, 22))
        self.comboBox.setDuplicatesEnabled(False)
        self.comboBox.setObjectName("comboBox")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.buttonBox = QtWidgets.QDialogButtonBox(NI6229)
        self.buttonBox.setGeometry(QtCore.QRect(265, 360, 291, 23))
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.RestoreDefaults|QtWidgets.QDialogButtonBox.Save)
        self.buttonBox.setObjectName("buttonBox")

        self.retranslateUi(NI6229)
        QtCore.QMetaObject.connectSlotsByName(NI6229)

    def retranslateUi(self, NI6229):
        _translate = QtCore.QCoreApplication.translate
        NI6229.setWindowTitle(_translate("NI6229", "Form"))
        self.label.setText(_translate("NI6229", "Input Maximum Analog Input Working Voltage"))
        self.label_2.setText(_translate("NI6229", "Input Range"))
        self.label_3.setText(_translate("NI6229", "Time for Multichannel Measurements"))
        self.comboBox.setItemText(0, _translate("NI6229", "90 ppm"))
        self.comboBox.setItemText(1, _translate("NI6229", "30 ppm"))
        self.comboBox.setItemText(2, _translate("NI6229", "15 ppm"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    NI6229 = QtWidgets.QMainWindow()
    ui4 = Ui_NI6229()
    ui4.setupUi4(NI6229)
    NI6229.show()
    sys.exit(app.exec_())
