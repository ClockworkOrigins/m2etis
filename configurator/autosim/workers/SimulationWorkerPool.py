#!/usr/bin/env python
from autosim.workers.SimulationWorker import SimulationWorker

__author__ = 'Andreas M. Wahl'

from Queue import Queue
import logging


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class SimulationWorkerPool:

        def __init__(self, simulation_nodes, simulation_data):

            self.simulation_nodes = simulation_nodes
            self.simulation_queue = Queue()

            for item in simulation_data:
                self.simulation_queue.put(item)

            self.result_queue = Queue()
            self.fail_queue = Queue()
            self.simulation_workers = []

            logger.info("Creating " + str(len(simulation_nodes)) + " simulation worker(s).")
            for node in self.simulation_nodes:
                    worker = SimulationWorker(node, self.simulation_queue, self.result_queue, self.fail_queue)
                    self.simulation_workers.append(worker)

        def run(self):
            """
            Run simulations on this simulation worker pool.
            """
            for worker in self.simulation_workers:
                worker.start()

        def await(self):
            """
            Await completion of all queued simulations and retrieve their results.
            :return: list containing the results of successful simulations as first element and a list of all failed simulations as second element
            """
            self.simulation_queue.join()
            return self.result_queue, self.fail_queue
