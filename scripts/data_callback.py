# It is mandatory to enable a data callback - for counting purposes
# This is the only method since writing values to the globals() in python from C is pretty much unreliable
import variables

def data_callback(count_r, angle_r, delta_r):
	if angle_r not in variables.__DReserved_totals:
		variables.__DReserved_totals[angle_r] = variables.CBData(count_r, delta_r)
	else:
		oldvals_c = variables.__DReserved_totals[angle_r].count
		oldvals_d = variables.__DReserved_totals[angle_r].delta
		variables.__DReserved_totals[angle_r] =  variables.CBData(count_r + oldvals_c, delta_r + oldvals_d)