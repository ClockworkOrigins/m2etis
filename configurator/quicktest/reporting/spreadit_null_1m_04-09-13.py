__author__ = 'amw'

from quicktest.Reporting import plot3d, plot2d, create_dataset, create_plot_matrix

# Initialise database config
db_config = {"hostname": "localhost", "port": "27017", "db_name": "simulations", "collection_name": "04-09-13"}


# Create plot
tag = "spreadit_null_1m_040913"
x_param = "parameters.numSubs"
y_param = "parameters.numToSend_0"
z_param = "results.<Channel 0>: Latency - All nodes - Avgmean"
filter_list = [{"parameters.packetSize": "16B"},
               {"parameters.packetSize": "32B"},
               {"parameters.packetSize": "64B"},
               {"parameters.packetSize": "128B"},
               {"parameters.packetSize": "1024B"}
               ]
plot3d(db_config, tag, x_param, y_param, z_param, filter_list, 2, 3, "Spreadit Null: Average Latency to all nodes")


y_param = "results.<Channel 0>: Latency - All nodes - Avgmean"

filter_list = [{"parameters.packetSize": "16B", "parameters.numToSend_0": "1"},
               {"parameters.packetSize": "16B", "parameters.numToSend_0": "3"},
               {"parameters.packetSize": "16B", "parameters.numToSend_0": "5"},
               {"parameters.packetSize": "16B", "parameters.numToSend_0": "7"},
               {"parameters.packetSize": "16B", "parameters.numToSend_0": "10"},
               {"parameters.packetSize": "16B", "parameters.numToSend_0": "15"},
               {"parameters.packetSize": "16B", "parameters.numToSend_0": "20"},
               {"parameters.packetSize": "16B", "parameters.numToSend_0": "30"}
               ]

# generate the 2d plots. the arguments "numCols" and "numRows" describe the layout of the resulting matrix of plots
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Spreadit Null: Average Latency to all nodes")

y_param = "results.<Channel 0>: Latency - All nodes - Minmean"
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Spreadit Null: Minimum Latency to all nodes")

y_param = "results.<Channel 0>: Latency - All nodes - Maxmean"
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Spreadit Null; Maximum Latency to all nodes")

