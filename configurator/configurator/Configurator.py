import os
import yaml
import logging
import strategyCollector
import parameterCollector
import copy
from include_generator.IncludeGenerator import IncludeGenerator
from autosim.SimulationAutomator import SimulationAutomator
import pprint

from missile.classes.event import Event
from missile.classes.strategy import Strategy
from missile.classes.application import Application
from missile.classes.network import  Network

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)


class Configurator:

    def __init__(self, config, parameter_collector=parameterCollector, strategy_collector=strategyCollector):

        self.application = yaml.load(open(config["path_to_application_description"], "r"))

        self.network = yaml.load(open(config["path_to_network_description"], "r"))

        self.strategy_repository = []
        for strategy_file in os.listdir(config["path_to_strategy_repository"]):
            if strategy_file.endswith(".mssl"):
                strategy = yaml.load(file(config["path_to_strategy_repository"] + "/" + strategy_file, "r"))
                self.strategy_repository.append(strategy)

        self.m2etis_library_directory = config["m2etis_library_directory"]
        self.m2etis_simulator_directory = config["m2etis_simulator_directory"]

        self.simulation_automator_config = yaml.load(open(config["simulation_automator_config"], "r"))

        self.parameter_collector = parameter_collector

        self.strategy_collector = strategy_collector

    def configure_event(self, event_file):

        LOG.info("Configuring event type from file: " + str(event_file))

        with file(event_file, "r") as stream:
            event = yaml.load(stream)

        if event.validate():

            strategies = self.strategy_collector.collect_strategies(event, self.strategy_repository)
            parameters = self.parameter_collector.collect_parameters(event, self.application)

            parameters["burstAmount"] = 10
            parameters["disableOverlay"] = "true"

            #self._generate_includes(event, strategies)
            #self._compile_m2etis()
            #self._compile_simulator()
            results = self._run_simulations(parameters, strategies)
            print self._get_strategies_for_minimum_metric(results ,"Latency")

    def _get_strategies_for_minimum_metric(self, simulation_results, metric):

        optimum_stratgies = None
        minimum_metric = float("inf")

        for result in simulation_results:
            if result["results"][metric] < minimum_metric:
                minimum_metric = result["results"][metric]
                optimum_stratgies = result["strategies"]

        return optimum_stratgies

    def _generate_includes(self, event, strategies):

        config = {
                "template_directory": "../include_generator/templates",
                "defines": [{"name": "WITH_SIM", "prefix": "SIM_", "network_types": ["m2etis::net::NetworkType<m2etis::net::OMNET>"]}],
                "input": [
                    {"event": event, "strategies": strategies, "defines": ["WITH_SIM"]},
                    {"event": event, "strategies": strategies, "defines": ["WITH_SIM"]},
                    {"event": event, "strategies": strategies, "defines": ["WITH_SIM"]}
                ]
        }

        generator = IncludeGenerator(config)
        generated_results = generator.generate()

        channel_name = open(self.m2etis_library_directory + "/include/config/GeneratedChannelName.h", "w")
        channel_name.write(generated_results[0][0])
        channel_name.close()

        event_types = open(self.m2etis_library_directory + "/include/config/GeneratedEventTypes.h", "w")
        event_types.write(generated_results[1])
        event_types.close()

        channel_configuration = open(self.m2etis_library_directory + "/include/config/GeneratedChannelConfiguration.h", "w")
        channel_configuration.write(generated_results[2])
        channel_configuration.close()

        message_configuration = open(self.m2etis_library_directory + "/include/config/GeneratedMessageConfiguration.h", "w")
        message_configuration.write(generated_results[3])
        message_configuration.close()

        return generated_results[0][1]

    def _compile_m2etis(self):

        current_working_directory = os.getcwd()

        os.chdir(self.m2etis_library_directory)
        os.system("make clean")
        os.system("make -j6")

        os.chdir(current_working_directory)

    def _compile_simulator(self):

        current_working_directory = os.getcwd()

        os.environ["PATH"] += os.pathsep + os.path.abspath(self.m2etis_simulator_directory) + "/omnetpp-4.3rc1/bin"
        os.chdir(self.m2etis_simulator_directory + "/m2etis")
        os.system("make clean")
        os.system("make -j6")

        os.chdir(current_working_directory)

    def _run_simulations(self, parameters, strategy_combinations):

        simulator = SimulationAutomator(self.simulation_automator_config)

        simulation_scenarios = []
        for strategy_combination in strategy_combinations:

            simulation_parameters = {}
            for parameter in parameters:

                if parameter == "numToSend":
                    simulation_parameters["**.numToSend_" + str(strategy_combinations.index(strategy_combination))] = parameters[parameter]
                elif parameter == "burstAmount":
                    simulation_parameters["**.burstAmount_" + str(strategy_combinations.index(strategy_combination))] = parameters[parameter]
                elif parameter == "burstFrequency":
                    simulation_parameters["**.burstFrequency_" + str(strategy_combinations.index(strategy_combination))] = parameters[parameter]
                elif parameter == "burstDuration":
                    simulation_parameters["**.burstDuration_" + str(strategy_combinations.index(strategy_combination))] = parameters[parameter]
                elif parameter == "chanceToUnsubscribe":
                    simulation_parameters["**.chanceToUnsubscribe_" + str(strategy_combinations.index(strategy_combination))] = parameters[parameter]
                elif parameter == "timeToUnsubscribe":
                    simulation_parameters["**.timeToUnsubscribe_" + str(strategy_combinations.index(strategy_combination))] = parameters[parameter]
                elif parameter == "timeToSubscribe":
                    simulation_parameters["**.timeToSubscribe_" + str(strategy_combinations.index(strategy_combination))] = parameters[parameter]
                elif parameter == "disableOverlay":
                    simulation_parameters["**.disableOverlay_" + str(strategy_combinations.index(strategy_combination))] = parameters[parameter]
                else:
                    simulation_parameters[parameter] = parameters[parameter]

            simulation_scenarios.append(simulation_parameters)
            print len(simulation_scenarios)
        #results, fails = simulator.run(simulation_scenarios, "test_configurator_1")
        #simulator.persistence.persist_to_database(results)

        results = [
            {
                "parameters": 1,
                "results": {"Latency": 10},
                "strategies": 1

            },
            {
                "parameters": 1,
                "results": {"Latency": 1},
                "strategies": 2

            }
        ]

        return results

    # def configureEvent(self, event_file, cost_model=None):
    #
    #     logger.info("Configuring event type from file: " + str(event_file))
    #
    #     with file(event_file, "r") as stream:
    #         event = yaml.load(stream)
    #         event.validate()
    #
    #     strategies = self.strategy_collector.collect_strategies(event, self.available_strategies)
    #     if cost_model is None:
    #         parameters = self.parameter_collector.collect_parameters(event)
    #
    #     else:
    #         pass


    # def calculate_cost_model(self, application):
    #     pass

