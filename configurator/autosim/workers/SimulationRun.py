#!/usr/bin/env python
from autosim.ssh.SSHClient import SSHClient

__author__ = 'Andreas M. Wahl'

import threading
import uuid
import os
import logging
#from autosim import ParameterDictionary


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class SimulationRun(threading.Thread):

    def __init__(self, simulation_node, simulation_scenario):

        threading.Thread.__init__(self)
        self.simulation_node = simulation_node
        self.simulation_scenario = simulation_scenario
        self.ssh_client = SSHClient(hostname=self.simulation_node["connection"]["hostname"],
                                    port=self.simulation_node["connection"]["port"],
                                    username=self.simulation_node["connection"]["username"],
                                    key_filename=self.simulation_node["connection"]["keyfile"])
        self.uuid = "m2etis_" + str(uuid.uuid1())

    def run(self):
        """
        Run a simulation using a (remote) simulator instance.
        :return: simulation results if the simulation was successful, "None" otherwise
        """
        try:
            # Create ini file locally
            ini_content = self._generate_ini_file()
            local_ini_file = open(self.uuid, 'w')
            local_ini_file.write(ini_content)
            local_ini_file.close()

            # Copy ini file to the remote node
            self.ssh_client.connect()
            self.ssh_client.copy_file(self.simulation_node["simulator"]["runDirectory"] + "/" + self.uuid, self.uuid)

            # Run simulator on remote node
            chdir_command = "cd " + self.simulation_node["simulator"]["runDirectory"]
            run_command = self.simulation_node["simulator"][
                              "binaryPath"] + " -uCmdenv " + "-f" + self.uuid + " -c" + self.uuid
#            run_command = "python SimulatorMock.py " + self.simulation_node["simulator"]["runDirectory"] + " " + self.uuid
            inpt, outpt, errpt = self.ssh_client.execute(chdir_command + ";" + run_command)

            # Wait for simulation to finish
            output = outpt.read()
            errput = errpt.read()

            results = None
            # Check if simulation was successful
            if not self.ssh_client.stat(self.simulation_node["simulator"][
                "runDirectory"] + "/results/" + self.uuid + "-0.sca"):
                logger.error("Simulation failed!" + "\n" + "STDOUT:\n" + output + "\nSTDERR:\n" + errput + "\nINICONTENT:\n" + ini_content)
                return None

            # Collect results from remote node
            raw_results = self.ssh_client.read_file(
                self.simulation_node["simulator"]["runDirectory"] + "/results/" + self.uuid + "-0.sca")
            results = self._parse_raw_results(raw_results)

        except Exception:
            pass
        finally:
            # Cleanup:

            # Remove ini file (local + remote)
            self.ssh_client.delete_file(self.simulation_node["simulator"]["runDirectory"] + "/" + self.uuid)
            os.remove(self.uuid)

            # Remove remote output files
            files = [self.simulation_node["simulator"]["runDirectory"] + "/results/" + self.uuid + "-0.sca",
                     self.simulation_node["simulator"]["runDirectory"] + "/results/" + self.uuid + "-0.vci",
                     self.simulation_node["simulator"]["runDirectory"] + "/results/" + self.uuid + "-0.vec"]

            for file in files:
                if self.ssh_client.stat(file):
                    self.ssh_client.delete_file(file)

            self.ssh_client.disconnect()
            return results

    def _generate_ini_file(self):

        ini_content = ""
        ini_content += "[Config " + self.uuid + "]\n"

        for parameter in self.simulation_scenario:
            #if parameter in ParameterDictionary.PARAMETERS:
#                ini_content += ParameterDictionary.PARAMETERS[parameter] + " = " + str(self.simulation_scenario[parameter]) + "\n"
 #           else:
            ini_content += parameter + " = " + str(self.simulation_scenario[parameter]) + "\n"

        ini_content += "include ./omnetpp.ini\n"
        ini_content += "include ./default.ini\n"

        return ini_content

    def _parse_raw_results(self, raw_results):

        results = {}

        for line in raw_results:

            if not line.find("TupleFeeder") == -1:
                tokens = line.split()
                attribute_length = len(tokens) - 4
                attribute = ""

                for i in range(3, attribute_length + 3):
                    attribute += tokens[i] + " "

                attribute = attribute[:-2]
                value = tokens[len(tokens) - 1]
                results[attribute] = value

        return results
