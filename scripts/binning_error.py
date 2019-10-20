import numpy as np
import matplotlib.pyplot as pp

x = np.random.randn(10000)
counts,bin_edges = np.histogram(x,20)
bin_centres = (bin_edges[:-1] + bin_edges[1:])/2.
err = np.random.rand(bin_centres.size)*100
pp.errorbar(bin_centres, counts, yerr=err, fmt='o')

pp.show()