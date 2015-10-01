#!/usr/bin/env python
__author__ = 'Andreas M. Wahl'

from autosim.workers.SimulationWorkerPool import SimulationWorkerPool


class SimulationAutomator:

    def __init__(self, config):
        """
        :param config: simulation automation config
        """
        self.config = config

    def run(self, simulations, tag):
        """
        Run simulations, tag them and retrieve the simulation results.
        :param simulation_parameters: simulation scenarios
        :param tag: custom tag to identify simulation results from a specific run
        :return: list containing the results of successful simulations as first element and a list of all failed simulations as second element
        """

        # Create list of simulation data arrays
        #simulation_data = []
        for simulation in simulations:
            simulation["results"] = None
            simulation["tag"] = tag
            #simulation_data.append({"parameters": simulation_parameter, "results": None, "tag": tag})

        # Distribute work
        simulation_workers = SimulationWorkerPool(self.config["simulation_nodes"], simulations)
        simulation_workers.run()

        # Collect results
        result_queue, fail_queue = simulation_workers.await()

        results = []
        fails = []
        while not result_queue.empty():
            results.append(result_queue.get())

        for item in list(fail_queue.queue):
            fails.append(item)

        return results, fails


