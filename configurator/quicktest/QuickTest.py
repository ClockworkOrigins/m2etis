__author__ = 'Andreas M. Wahl'

from autosim.SimulationAutomator import SimulationAutomator
from configurator.ranges.RangeGenerator import RangeGenerator
from configurator.visualization.Plotter import Plotter
from configurator.persistence.PersistenceManager import PersistenceManager
import yaml

from configurator.util.util import sanitize_results, sanitize_dict


class QuickTest:

    def __init__(self, simulations_file, config_file):

        self.config = yaml.load(open(config_file))

        self.generator = RangeGenerator(yaml.load(open(simulations_file)))
        self.automator = SimulationAutomator(self.config["SimulationAutomator"])

        self.persistence = PersistenceManager(self.config["Persistence"]["database"])
        self.plotter = Plotter(self.persistence)

    def setup(self):
        self.persistence.start_db()

    def run_simulations(self, tag):
        simulation_scenarios = self.generator.generate_memory()
	#print len(simulation_scenarios)

        automator_input = []
        for scenario in simulation_scenarios:
            automator_entry = {}
            automator_entry["parameters"] = scenario
            automator_entry["strategies"] = None
            automator_input.append(automator_entry)

        results, fails = self.automator.run(automator_input, tag)
        self.persistence.initialise_database_client()
        sanitized_results = sanitize_results(results)
        #print sanitized_results
        self.persistence.persist_to_database(self.config["Persistence"]["database"]["collection_name"], sanitized_results)
        return results, fails

    def plot2d(self, tag, x_param, y_param, filter, output_filename, title="", x_label="", y_label=""):
        self.persistence.initialise_database_client()
        self.plotter.plot2d_from_database(tag, x_param, y_param, filter, output_filename, title, x_label, y_label)

    def plot3d(self, tag, x_param, y_param, z_param, filter, output_filename, title="", x_label="", y_label="", z_label=""):
        self.persistence.initialise_database_client()
        self.plotter.plot3d_from_database(tag, x_param, y_param, z_param, filter, output_filename, title, x_label, y_label, z_label)


    def teardown(self):
        self.persistence.shutdown_db()
