#!/usr/bin/env python
__author__ = 'Andreas M. Wahl'

import sys
import os
import random


class SimulatorMock:

    def __init__(self, runDirectory, uuid):
        self.runDirectory = runDirectory
        self.uuid = uuid

    def writeOutput(self):
        strings = ['scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: First contact - Avg.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: First contact - Max.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: First contact - Min.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: Latency - All nodes - Avg.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: Latency - All nodes - Max.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: Latency - All nodes - Min.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: Latency - One node - Avg.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: Latency - One node - Max.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: Latency - One node - Min.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: Received packets.mean"',
                   'scalar SimpleUnderlayNetwork.globalObserver.globalStatistics 	"TupleFeeder: Sent packets.mean"']
        fileContent = ""

        for string in strings:
            fileContent += string + " " + str(random.random()) + "\n"

        fileName = self.runDirectory + "/results/" + str(self.uuid) + "-0.sca"
        dir = os.path.dirname(fileName)

        if not os.path.exists(dir):
            os.makedirs(dir)

        fileObject = open(fileName, 'w')
        fileObject.write(fileContent)

        print "End."
        sys.stdout.flush()

mock = SimulatorMock(sys.argv[1], sys.argv[2])
mock.writeOutput()