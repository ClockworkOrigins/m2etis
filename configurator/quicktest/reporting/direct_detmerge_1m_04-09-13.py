__author__ = 'amw'

from quicktest.Reporting import plot3d, plot2d, create_dataset, create_plot_matrix

# Initialise database config
db_config = {"hostname": "localhost", "port": "27017", "db_name": "simulations", "collection_name": "validation_050913"}


tag = "direct_detmerge_1m_040913-2"
x_param = "parameters.numSubs"
y_param = "parameters.numToSend_6"
z_param = "results.<Channel 6>: Latency - All nodes - Avgmean"
filter_list = [{"parameters.packetSize": "16B"},
               {"parameters.packetSize": "32B"},
               {"parameters.packetSize": "64B"},
               {"parameters.packetSize": "128B"},
               {"parameters.packetSize": "1024B"}
               ]
plot3d(db_config, tag, x_param, y_param, z_param, filter_list, 2, 3, "Direct Detmerge Average Latency All Nodes")


x_param = "parameters.numSubs"
y_param = "results.<Channel 6>: Latency - All nodes - Minmean"

filter_list = [{"parameters.packetSize": "16B", "parameters.numToSend_6": "1"},
               {"parameters.packetSize": "16B", "parameters.numToSend_6": "3"},
               {"parameters.packetSize": "16B", "parameters.numToSend_6": "5"},
               {"parameters.packetSize": "16B", "parameters.numToSend_6": "7"},
               {"parameters.packetSize": "16B", "parameters.numToSend_6": "10"},
               {"parameters.packetSize": "16B", "parameters.numToSend_6": "15"},
               {"parameters.packetSize": "16B", "parameters.numToSend_6": "20"},
               {"parameters.packetSize": "16B", "parameters.numToSend_6": "30"}
               ]

plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct Detmerge Minimum Latency All Nodes")

y_param = "results.<Channel 6>: Latency - All nodes - Maxmean"
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct Detmerge Maximum Latency All Nodes")

y_param = "results.<Channel 6>: Latency - All nodes - Avgmean"
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct Detmerge Average Latency All Nodes")

