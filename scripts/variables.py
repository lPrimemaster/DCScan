# These variables are stored to be used in the backend
# They are read only in the client side (Python)

import collections
CBData = collections.namedtuple('CBData', ['count', 'delta'])

# All the point data of the counts is stored here
DReserved_totals = dict()

# The Current status array for multiple status values
# 0 - Not Working / Warning
# 1 - Functional
DReserved_status = dict()
