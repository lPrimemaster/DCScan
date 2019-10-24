## Transition Window
## Synchronization of Devices

from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Dialog(object):
    def setupUi5(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(400, 151)
        self.progressBar = QtWidgets.QProgressBar(Dialog)
        self.progressBar.setGeometry(QtCore.QRect(100, 80, 221, 23))
        self.progressBar.setProperty("value", 24)
        self.progressBar.setObjectName("progressBar")
        self.label = QtWidgets.QLabel(Dialog)
        self.label.setGeometry(QtCore.QRect(130, 40, 141, 16))
        self.label.setObjectName("label")

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Signal Process"))
        self.label.setText(_translate("Dialog", "Waiting for synchronization"))


if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    Signal_Process = QtWidgets.QDialog()
    ui5 = Ui_Dialog()
    ui5.setupUi5(Signal_Process)
    Signal_Process.show()
    sys.exit(app.exec_())