#!/usr/bin/env python
from autosim.workers.SimulationRun import SimulationRun

__author__ = 'Andreas M. Wahl'

from threading import Thread
import logging


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class SimulationWorker(Thread):

    def __init__(self, simulation_node, parameter_queue, result_queue, fail_queue):

        Thread.__init__(self)
        self.simulation_node = simulation_node
        self.parameter_queue = parameter_queue
        self.result_queue = result_queue
        self.fail_queue = fail_queue

    def run(self):
        """
        Run simulations using this simulation worker instance.
        """
        while not self.parameter_queue.empty():
            simulation_data = self.parameter_queue.get()
            # Progress notification
            logger.info(self.simulation_node["connection"]["hostname"] + ": Simulation taken from queue. " + str(self.parameter_queue.qsize()) + " simulation(s) remaining in queue.")
            simulation_run = SimulationRun(self.simulation_node, simulation_data["parameters"])
            simulation_results = simulation_run.run()

            if simulation_results is None:
                self.fail_queue.put(simulation_data)
            elif len(simulation_results) == 0:
                self.fail_queue.put(simulation_data)
            else:
                #combinedResults = OrderedDict(simulationParameters.items() + simulationResults.items())
                simulation_data["results"] = simulation_results
                self.result_queue.put(simulation_data)

            self.parameter_queue.task_done()
        logger.info(self.simulation_node["connection"]["hostname"] + ": Queue emtpy. Worker retired.")
