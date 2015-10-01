__author__ = 'amw'

from configurator.persistence.PersistenceManager import PersistenceManager
from Reporting import plot3d, plot2d, create_dataset, create_plot_matrix

# Initialise database config
db_config = {"hostname": "localhost", "port": "27017", "db_name": "sianwahl_test", "collection_name": "test"}

#
# 2d example
#

tag = "channel0_all_no_flux" # name of the database tag of the dataset to be plotted
x_param = "parameters.numPubSubs" # name of the x-parameter
y_param = "results.Latency - All nodes - Avgmean" # name of the y-parameter
# list of filters. each filter specifies the variables to be used in the corresponding subplot.
filter_list = [{"parameters.packetSize": "32B", "parameters.numToSend_0": "30"},
               {"parameters.packetSize": "32B", "parameters.numToSend_0": "1"},
               {"parameters.packetSize": "32B", "parameters.numToSend_0": "5"},
               {"parameters.packetSize": "32B", "parameters.numToSend_0": "10"},
               {"parameters.packetSize": "32B", "parameters.numToSend_0": "15"},
               {"parameters.packetSize": "32B", "parameters.numToSend_0": "20"},
               {"parameters.packetSize": "32B", "parameters.numToSend_0": "25"}
               ]
# generate the 2d plots. the arguments "numCols" and "numRows" describe the layout of the resulting matrix of plots
plot2d(db_config, tag, x_param, y_param, filter_list, 4, 2, "Channel0 2d")


#
# 3d example
#

y_param = "parameters.numToSend_0"
z_param = "results.Latency - All nodes - Avgmean"
filter_list = [{"parameters.packetSize": "16B"},
               {"parameters.packetSize": "32B"},
               ]
plot3d(db_config, tag, x_param, y_param, z_param, filter_list, 1, 2, "Channel0 3d")

