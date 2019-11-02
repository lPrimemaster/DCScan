# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainWindow.ui'
#
# Created by: PyQt5 UI code generator 5.13.0
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1280, 720)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        MainWindow.setFocusPolicy(QtCore.Qt.NoFocus)
        MainWindow.setStyleSheet("#centralwidget\n"
"{\n"
"    background-color: #333;\n"
"}\n"
"\n"
"QPushButton\n"
"{\n"
"    background: #ff0000;\n"
"    border-radius:5px;\n"
"    font: 15px \"Rockwell\";\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background: #ff6600;\n"
"    border-radius:5px;\n"
"    font: 15px \"Rockwell\";\n"
"}\n"
"\n"
"QMenuBar\n"
"{\n"
"    background-color: #111;\n"
"    padding: 2px;\n"
"    text-align: center;\n"
"}\n"
"\n"
"QMenuBar::item\n"
"{\n"
"    color: white;\n"
"    padding: 7px;\n"
"    border-radius: 2px;\n"
"    text-align: center;\n"
"}\n"
"\n"
"QMenuBar::item:selected \n"
"{\n"
"    background: red;\n"
"}\n"
"\n"
"QMenuBar::item:pressed \n"
"{  \n"
"    background: white;\n"
"    color: black;\n"
"}\n"
"\n"
"QMenu\n"
"{\n"
"    background-color: #111;\n"
"    text-align: center;\n"
"}\n"
"\n"
"QMenu::item\n"
"{\n"
"    color: white;\n"
"    text-align: center;\n"
"}\n"
"\n"
"QMenu::item:selected \n"
"{\n"
"    background: red;\n"
"}\n"
"\n"
"QMenu::item:pressed \n"
"{  \n"
"    background: white;\n"
"    color: black;\n"
"}")
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayoutWidget_2 = QtWidgets.QWidget(self.centralwidget)
        self.verticalLayoutWidget_2.setGeometry(QtCore.QRect(10, 10, 241, 651))
        self.verticalLayoutWidget_2.setObjectName("verticalLayoutWidget_2")
        self.verticalSide = QtWidgets.QVBoxLayout(self.verticalLayoutWidget_2)
        self.verticalSide.setContentsMargins(0, 0, 0, 0)
        self.verticalSide.setObjectName("verticalSide")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout()
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.gridLayout_2 = QtWidgets.QGridLayout()
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.pushButton = QtWidgets.QPushButton(self.verticalLayoutWidget_2)
        self.pushButton.setObjectName("pushButton")
        self.gridLayout_2.addWidget(self.pushButton, 0, 0, 1, 1)
        self.pushButton_2 = QtWidgets.QPushButton(self.verticalLayoutWidget_2)
        self.pushButton_2.setObjectName("pushButton_2")
        self.gridLayout_2.addWidget(self.pushButton_2, 1, 0, 1, 1)
        self.pushButton_3 = QtWidgets.QPushButton(self.verticalLayoutWidget_2)
        self.pushButton_3.setObjectName("pushButton_3")
        self.gridLayout_2.addWidget(self.pushButton_3, 0, 1, 1, 1)
        self.pushButton_4 = QtWidgets.QPushButton(self.verticalLayoutWidget_2)
        self.pushButton_4.setObjectName("pushButton_4")
        self.gridLayout_2.addWidget(self.pushButton_4, 1, 1, 1, 1)
        self.verticalLayout_3.addLayout(self.gridLayout_2)
        self.verticalSide.addLayout(self.verticalLayout_3)
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.verticalSide.addItem(spacerItem)
        self.verticalLayout_5 = QtWidgets.QVBoxLayout()
        self.verticalLayout_5.setObjectName("verticalLayout_5")
        self.verticalSide.addLayout(self.verticalLayout_5)
        self.gridLayoutWidget = QtWidgets.QWidget(self.centralwidget)
        self.gridLayoutWidget.setGeometry(QtCore.QRect(260, 549, 1011, 111))
        self.gridLayoutWidget.setObjectName("gridLayoutWidget")
        self.gridBottom = QtWidgets.QGridLayout(self.gridLayoutWidget)
        self.gridBottom.setContentsMargins(0, 0, 0, 0)
        self.gridBottom.setObjectName("gridBottom")
        self.verticalLayoutWidget = QtWidgets.QWidget(self.centralwidget)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(260, 10, 1011, 531))
        self.verticalLayoutWidget.setObjectName("verticalLayoutWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.verticalLayoutWidget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.graphWidget = PlotWidget(self.verticalLayoutWidget)
        self.graphWidget.setObjectName("graphWidget")
        self.verticalLayout.addWidget(self.graphWidget)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1280, 34))
        self.menubar.setObjectName("menubar")
        self.menuRun = QtWidgets.QMenu(self.menubar)
        self.menuRun.setObjectName("menuRun")
        self.menuHelp = QtWidgets.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        self.menuAbout = QtWidgets.QMenu(self.menubar)
        self.menuAbout.setObjectName("menuAbout")
        self.menuSettings = QtWidgets.QMenu(self.menubar)
        self.menuSettings.setObjectName("menuSettings")
        self.menuConfigure = QtWidgets.QMenu(self.menubar)
        self.menuConfigure.setObjectName("menuConfigure")
        self.menuExport = QtWidgets.QMenu(self.menubar)
        self.menuExport.setObjectName("menuExport")
        self.menuFile = QtWidgets.QMenu(self.menuExport)
        self.menuFile.setObjectName("menuFile")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionAcquire_w_current_settings = QtWidgets.QAction(MainWindow)
        self.actionAcquire_w_current_settings.setObjectName("actionAcquire_w_current_settings")
        self.actionStop_Acquisition = QtWidgets.QAction(MainWindow)
        self.actionStop_Acquisition.setObjectName("actionStop_Acquisition")
        self.actionSelf_Test = QtWidgets.QAction(MainWindow)
        self.actionSelf_Test.setObjectName("actionSelf_Test")
        self.actionAcquisition = QtWidgets.QAction(MainWindow)
        self.actionAcquisition.setObjectName("actionAcquisition")
        self.actionControl = QtWidgets.QAction(MainWindow)
        self.actionControl.setObjectName("actionControl")
        self.actionProcess = QtWidgets.QAction(MainWindow)
        self.actionProcess.setObjectName("actionProcess")
        self.actionAxis = QtWidgets.QAction(MainWindow)
        self.actionAxis.setObjectName("actionAxis")
        self.actionOptical_Enconder = QtWidgets.QAction(MainWindow)
        self.actionOptical_Enconder.setObjectName("actionOptical_Enconder")
        self.actionGeneral = QtWidgets.QAction(MainWindow)
        self.actionGeneral.setObjectName("actionGeneral")
        self.actionCSV = QtWidgets.QAction(MainWindow)
        self.actionCSV.setObjectName("actionCSV")
        self.actionCsv = QtWidgets.QAction(MainWindow)
        self.actionCsv.setObjectName("actionCsv")
        self.actionHtml = QtWidgets.QAction(MainWindow)
        self.actionHtml.setObjectName("actionHtml")
        self.actionsql = QtWidgets.QAction(MainWindow)
        self.actionsql.setObjectName("actionsql")
        self.menuRun.addAction(self.actionAcquire_w_current_settings)
        self.menuRun.addAction(self.actionStop_Acquisition)
        self.menuRun.addAction(self.actionSelf_Test)
        self.menuSettings.addAction(self.actionAcquisition)
        self.menuSettings.addAction(self.actionControl)
        self.menuSettings.addAction(self.actionProcess)
        self.menuConfigure.addAction(self.actionGeneral)
        self.menuConfigure.addAction(self.actionAxis)
        self.menuConfigure.addAction(self.actionOptical_Enconder)
        self.menuFile.addAction(self.actionCsv)
        self.menuFile.addAction(self.actionHtml)
        self.menuFile.addAction(self.actionsql)
        self.menuExport.addAction(self.actionCSV)
        self.menuExport.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuRun.menuAction())
        self.menubar.addAction(self.menuSettings.menuAction())
        self.menubar.addAction(self.menuConfigure.menuAction())
        self.menubar.addAction(self.menuExport.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())
        self.menubar.addAction(self.menuAbout.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.pushButton.setText(_translate("MainWindow", "Start"))
        self.pushButton_2.setText(_translate("MainWindow", "Resume"))
        self.pushButton_3.setText(_translate("MainWindow", "Stop"))
        self.pushButton_4.setText(_translate("MainWindow", "Pause"))
        self.menuRun.setTitle(_translate("MainWindow", "Run"))
        self.menuHelp.setTitle(_translate("MainWindow", "Help"))
        self.menuAbout.setTitle(_translate("MainWindow", "About"))
        self.menuSettings.setTitle(_translate("MainWindow", "Settings"))
        self.menuConfigure.setTitle(_translate("MainWindow", "Configure"))
        self.menuExport.setTitle(_translate("MainWindow", "Export"))
        self.menuFile.setTitle(_translate("MainWindow", "File"))
        self.actionAcquire_w_current_settings.setText(_translate("MainWindow", "Start Acquisition"))
        self.actionStop_Acquisition.setText(_translate("MainWindow", "Stop Acquisition"))
        self.actionSelf_Test.setText(_translate("MainWindow", "Self Test"))
        self.actionAcquisition.setText(_translate("MainWindow", "Acquisition"))
        self.actionControl.setText(_translate("MainWindow", "Control"))
        self.actionProcess.setText(_translate("MainWindow", "Process"))
        self.actionAxis.setText(_translate("MainWindow", "Axis"))
        self.actionOptical_Enconder.setText(_translate("MainWindow", "Optical Encoder"))
        self.actionGeneral.setText(_translate("MainWindow", "General"))
        self.actionCSV.setText(_translate("MainWindow", "Quick Export"))
        self.actionCsv.setText(_translate("MainWindow", "csv"))
        self.actionHtml.setText(_translate("MainWindow", "html"))
        self.actionsql.setText(_translate("MainWindow", "sql"))
from pyqtgraph import PlotWidget
