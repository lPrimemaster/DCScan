# Embedded operation imports from C++
from DCS_Common import *
import DCS_Time as dcst
import DCS_Data as dcsd
import DCS_Control as dcsc
import variables

class BHandler():

	def __init__(self): pass

	def discardCounts(self):
		variables.DReserved_totals.clear()

	def stopServerAcquisition(self):
		dcsc.setServerAcquireState(0)

	def startServerAcquisition(self):
		dcsc.setServerAcquireState(1)

	def signalTerminate(self):
		pass # For now! TODO
