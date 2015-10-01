from string import Template
import yaml

generic_static_parameters_template = """
static_parameters:
  "*.underlayConfigurator.churnGeneratorTypes": "\\"oversim.common.NoChurn\\""
  "**.overlayType": "\\"oversim.overlay.pastry.PastryModules\\""
  "**.neighborCache.enableNeighborCache": "true"
  "**.numTiers": 2
  "**.tier1Type": "\\"m2etis.middleware.M2etisPubSub\\""
  "**.tier2Type": "\\"m2etis.applications.TupleFeeder.TupleFeederModules\\""
  "*.underlayConfigurator.measurementTime": "60s"
  "**.enableDrops": "false"
  "**.dropChance": 0
  "**.numRend": 0
  "**.largestKey": 4
  "**.udp.constantDelay": "30ms"
  "**.udp.jitter": 0.1
  "**.headerSize": "40B"
  "**.expectedCompression": 0.0
  "**.upstream": "1Mbps"
  "**.downstream": "6Mbps"
  "**.lossTimeout": "60s"
  "**.queueSize": "10MiB"
  "**.queueDisabled": "false"
  "**.startRoot": 1
  "**.endRoot": 1
  "**.rendezvousNode": 1
  "**.stopAvg": "59.0"
  "**.simulationResolution": "1ms"

  "**.waitForSubscribe": "5s"
  "**.waitForPublish": "2s"

  "SimpleUnderlayNetwork.churnGenerator*.channelTypes": "\\"m2etis.simple_m2etis_channel\\""

  "**.drawOverlayTopology": "true"
  "**.measureNetwInitPhase": "false"
  "**.useCommonAPIforward": "true"

  "**.channelCount": $channelCount
"""

channel_specific_static_parameters_template = """
  # Channel $id
  "**.numToSend_$id": 0
  "**.burstAmount_$id": 0
  "**.burstFrequency_$id": "0s"
  "**.burstDuration_$id": "0s"
  "**.disableOverlay_$id": $disable_overlay
  "**.timeToUnsubscribe_$id": "0s"
  "**.timeToSubscribe_$id": "0s"
  "**.chanceToUnsubscribe_$id": 0
  "**.dropChance_$id": 0
  "**.compensateDrop_$id": "false"
  "**.fluctuation_$id": 0
"""

channel_specific_static_parameters_template_current_channel = """
  # Channel $id
  "**.burstAmount_$id": 0
  "**.burstFrequency_$id": "0s"
  "**.burstDuration_$id": "0s"
  "**.disableOverlay_$id": $disable_overlay
  "**.timeToUnsubscribe_$id": "0s"
  "**.timeToSubscribe_$id": "0s"
  "**.chanceToUnsubscribe_$id": 0
  "**.dropChance_$id": 0
  "**.compensateDrop_$id": "false"
"""

generic_dynamic_parameters_template = """
dynamic_parameters:
  "**.numSubs":
    values: $numSubs_values
    unit: ""
  "**.numPubs":
    values: $numPubs_values
    unit: ""
  "**.numPubSubs":
    values: $numPubSubs_values
    unit: ""
  "**.packetSize":
    values: $packetSize_values
    unit: "B"
"""

channel_specific_dynamic_parameters_template = """
  "**.numToSend_$id":
    values: $numToSend_values
    unit: ""
  "**.fluctuation_$id":
    values: $fluctuation_values
    unit: ""
"""

generic_dependent_parameters_template = """
dependent_parameters:
   "**.targetOverlayTerminalNum":
     type: "sum"
     dependency: ["**.numSubs", "**.numPubs", "**.numPubSubs"]
     unit: ""
"""

class RangeMetaGenerator:

    def __init__(self, channels, output_path, numSubs_values, numPubs_values, numPubSubs_values, packetSize_values, numToSend_values, fluctuation_values):
        self.channels = channels
        self.output_path = output_path
        self.numSubs_values = numSubs_values
        self.numPubs_values = numPubs_values
        self.numPubSubs_values = numPubSubs_values
        self.packetSize_values = packetSize_values
        self.numToSend_values = numToSend_values
        self.fluctuation_values = fluctuation_values

    def generate_ranges(self):

        channel_count = len(self.channels)
        current_channel = 0

        for channel in self.channels:

            range_generator_config = ""

            t = Template(generic_static_parameters_template)
            range_generator_config += t.substitute(channelCount = channel_count)

            for i in range(0, channel_count):
                if "Scribe" in self.channels[i]:
                    disable_overlay = "\"false\""
                else:
                    disable_overlay ="\"true\""

                if i == current_channel:
                    t = Template(channel_specific_static_parameters_template_current_channel)
                    range_generator_config += t.substitute(id=i, disable_overlay=disable_overlay)
                else:
                    t = Template(channel_specific_static_parameters_template)
                    range_generator_config += t.substitute(id=i, disable_overlay=disable_overlay)

            t = Template(generic_dynamic_parameters_template)
            range_generator_config += t.substitute(numSubs_values=str(self.numSubs_values), numPubs_values=str(self.numPubs_values), numPubSubs_values=str(self.numPubSubs_values), packetSize_values=str(self.packetSize_values))

            t = Template(channel_specific_dynamic_parameters_template)
            range_generator_config += t.substitute({"id": current_channel, "numToSend_values": str(self.numToSend_values), "fluctuation_values": str(self.fluctuation_values)})

            t = Template(generic_dependent_parameters_template)
            range_generator_config += t.substitute()

            current_channel += 1

            loaded = yaml.load(range_generator_config)
            stream = file(self.output_path + "/" + channel + ".mtn", 'w')
            yaml.dump(loaded, stream)
