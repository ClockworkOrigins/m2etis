##!/home/i6stud/sianwahl/bootstrap/bin/python
__author__ = 'Andreas M. Wahl'

import logging
import yaml
from autosim.SimulationAutomator import SimulationAutomator


logging.basicConfig(level=logging.ERROR)
logger = logging.getLogger(__name__)

# initialize the automation tool by supplying a configuration file
config = yaml.load(open("../missile/examples/configs/unified_configuration.mssl", "r"))
sim = SimulationAutomator(config["SimulationAutomator"])

# run simulations
#simulations = yaml.load(open("../autosim/simulations.m2etis", "r"))
#results, fails = sim.run(simulations, "sample_scenario_1")

# check if there were failed simulations.
#if len(fails) > 0:
#    logger.error("Simulation(s) failed.")
   # sys.exit(1)

# process results:

# persist results
#sim.persistence.persist_to_file(results, "test.yaml")
#sim.persistence.persist_to_database(results)

# plot results
#sim.plotter.plot2d_from_memory(results, "parameters.numSubs", "results.Latency - All nodes - Maxmean", "mem_out.pdf", "Test Simulation", "Number of PubSubs", "Max. mean of latency to all nodes")
#sim.plotter.plot2d_from_file("test.yaml", "parameters.numSubs", "results.Latency - All nodes - Maxmean", "file_out.png")
#sim.plotter.plot2d_from_database("sample_scenario_1", "parameters.numSubs", "results.Latency - All nodes - Maxmean", "db_out.png")

# retrieve results for further processing, e.g. in a custom python script
#results_from_file = sim.persistence.retrieve_from_file("test.yaml")
#results_from_database = sim.persistence.retrieve_from_database({"tag": "sample_scenario_1"}, {"_id": 1}, [("_id", -1)])
#results_from_database_corresponding = sim.persistence.retrieve_from_database_corresponding_results("sample_scenario_1", "parameters.numPubSubs", "results.Latency - All nodes - Maxmean")


# optional: clear database
#sim.persistence.clear_database()
