# It is mandatory to enable a data callback - for counting purposes
import variables
# import pandas as pd

def data_callback(count_r, angle_r, delta_r):
	if angle_r not in variables.totals.count:		
		variables.totals.count[angle_r] = count_r
		variables.totals.delta[angle_r] = delta_r
	else:
		oldvals_c = variables.totals.count[angle_r]
		oldvals_d = variables.totals.delta[angle_r]
		variables.totals.count[angle_r] =  count_r + oldvals_c
		variables.totals.delta[angle_r] =  delta_r + oldvals_d
		# variables.totals[angle_r] =  variables.CBData(count_r, delta_r)

# data_callback( 5,  7.0, 0.8)
# data_callback( 9,  7.0, 0.8)
# data_callback(23,  9.0, 0.8)
# data_callback(17, 10.0, 0.8)
# data_callback(10, 11.0, 0.8)
# data_callback( 3, 12.0, 0.8)

# print(variables.totals)
# print(variables.totals[10.0])

# data_frame = pd.dataframe(variables.totals)
# data_frame.to_csv(r'modeled.csv')
# print(data_frame)