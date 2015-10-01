__author__ = 'amw'

from quicktest.Reporting import plot3d, plot2d, create_dataset, create_plot_matrix

# Initialise database config
db_config = {"hostname": "localhost", "port": "27017", "db_name": "simulations", "collection_name": "validation_050913"}

tag = "direct_gms_1m_040913-2"
x_param = "parameters.numSubs"
y_param = "parameters.numToSend_7"
z_param = "results.<Channel 7>: Latency - All nodes - Avgmean"
filter_list = [{"parameters.packetSize": "16B"},
               {"parameters.packetSize": "32B"},
               {"parameters.packetSize": "64B"},
               {"parameters.packetSize": "128B"},
               {"parameters.packetSize": "1024B"}
               ]
plot3d(db_config, tag, x_param, y_param, z_param, filter_list, 2, 3, "Direct GMS Average Latency All Nodes")

x_param = "parameters.numSubs" # name of the x-parameter
y_param = "results.<Channel 7>: Latency - All nodes - Minmean"

filter_list = [{"parameters.packetSize": "16B", "parameters.numToSend_7": "1"},
               {"parameters.packetSize": "16B", "parameters.numToSend_7": "3"},
               {"parameters.packetSize": "16B", "parameters.numToSend_7": "5"},
               {"parameters.packetSize": "16B", "parameters.numToSend_7": "7"},
               {"parameters.packetSize": "16B", "parameters.numToSend_7": "10"},
               {"parameters.packetSize": "16B", "parameters.numToSend_7": "15"},
               {"parameters.packetSize": "16B", "parameters.numToSend_7": "20"},
               {"parameters.packetSize": "16B", "parameters.numToSend_7": "30"}
               ]

plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct GMS Minimum Latency All Nodes")

y_param = "results.<Channel 7>: Latency - All nodes - Maxmean"
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct GMS Maximum Latency All Nodes")

y_param = "results.<Channel 7>: Latency - All nodes - Avgmean"
plot2d(db_config, tag, x_param, y_param, filter_list, 3, 3, "Direct GMS Average Latency All Nodes ")
