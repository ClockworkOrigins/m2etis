__author__ = 'amw'

from quicktest.Reporting import plot3d, plot2d, create_dataset, create_plot_matrix

# Initialise database config
db_config = {"hostname": "localhost", "port": "27017", "db_name": "simulations", "collection_name": "03-09-13"}


# Create plot
tag = "direct_mtp_1m_030913"
x_param = "parameters.numSubs"
y_param = "parameters.numToSend_5"
z_param = "results.<Channel 5>: Latency - All nodes - Avgmean"
filter_list = [{"parameters.packetSize": "16B"},
               {"parameters.packetSize": "32B"},
               {"parameters.packetSize": "64B"},
               {"parameters.packetSize": "128B"},
               {"parameters.packetSize": "1024B"}
               ]
plot3d(db_config, tag, x_param, y_param, z_param, filter_list, 2, 3, "Direct MTP: Average Latency to all nodes")


y_param = "results.<Channel 5>: Latency - All nodes - Avgmean"

filter_list = [{"parameters.packetSize": "16B", "parameters.numToSend_5": "1"},
               {"parameters.packetSize": "16B", "parameters.numToSend_5": "3"},
               {"parameters.packetSize": "16B", "parameters.numToSend_5": "5"},
               {"parameters.packetSize": "16B", "parameters.numToSend_5": "7"},
               {"parameters.packetSize": "16B", "parameters.numToSend_5": "10"},
               {"parameters.packetSize": "16B", "parameters.numToSend_5": "15"},
               {"parameters.packetSize": "16B", "parameters.numToSend_5": "20"},
               {"parameters.packetSize": "16B", "parameters.numToSend_5": "30"}
               ]

# generate the 2d plots. the arguments "numCols" and "numRows" describe the layout of the resulting matrix of plots
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct MTP: Average Latency to all nodes")

y_param = "results.<Channel 5>: Latency - All nodes - Minmean"
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct MTP: Minimum Latency to all nodes")

y_param = "results.<Channel 5>: Latency - All nodes - Maxmean"
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct MTP; Maximum Latency to all nodes")

