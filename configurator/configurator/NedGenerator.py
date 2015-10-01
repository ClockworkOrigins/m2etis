__author__ = 'sianwahl'

from string import Template


class NedGenerator:

    def __init__(self, number_of_channels):
        self.number_of_channels = number_of_channels

    def generate(self):
        return self._generate_tuplefeeder_ned(), self._generate_m2etis_ned()

    def _generate_tuplefeeder_ned(self):

        template = """
        //
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package m2etis.applications.TupleFeeder;

import oversim.common.BaseApp;
import oversim.common.ITier;

simple TupleFeeder extends BaseApp
{
    parameters:
        @class(TupleFeeder);
        int largestKey;            // largest key we can pick
        int numSubs;
        int numPubs;
        int numPubSubs;
        int numRend;
        int channelCount;
        double stopAvg;
        int waitForSubscribe @unit(s);
        int waitForPublish @unit(s);
        $channel_specific_parameters
}

module TupleFeederModules like ITier
{
    parameters:
        @display("i=block/segm");

    gates:
        input from_lowerTier;    // gate from the lower tier
        input from_upperTier;    // gate from the upper tier
        output to_lowerTier;     // gate to the lower tier
        output to_upperTier;     // gate to the upper tier
        input trace_in;          // gate for trace file commands
        input udpIn;
        output udpOut;
        input tcpIn;
        output tcpOut;

    submodules:
        tupleFeeder: TupleFeeder;

    connections allowunconnected:
        from_lowerTier --> tupleFeeder.from_lowerTier;
        to_lowerTier <-- tupleFeeder.to_lowerTier;
        trace_in --> tupleFeeder.trace_in;
        udpIn --> tupleFeeder.udpIn;
        udpOut <-- tupleFeeder.udpOut;
}
        """
        channel_specific_parameters = ""
        for i in range(0, self.number_of_channels):
            channel_specific_parameters += "int numToSend_" + str(i) + ";\n\t"
            channel_specific_parameters += "int burstAmount_" + str(i) + ";\n\t"
            channel_specific_parameters += "int burstFrequency_" + str(i) + " @unit(s);\n\t"
            channel_specific_parameters += "int burstDuration_" + str(i) + " @unit(s);\n\t"
            channel_specific_parameters += "int chanceToUnsubscribe_" + str(i) + ";\n\t"
            channel_specific_parameters += "int timeToUnsubscribe_" + str(i) + " @unit(s);\n\t"
            channel_specific_parameters += "int timeToSubscribe_" + str(i) + " @unit(s);\n\t"
            channel_specific_parameters += "int dropChance_" + str(i) + ";\n\t"
            channel_specific_parameters += "bool compensateDrop_" + str(i) + ";\n\t"
            channel_specific_parameters += "double fluctuation_" + str(i) + ";\n\t"

        template_prepared = Template(template)
        result = template_prepared.substitute(
            channel_specific_parameters=channel_specific_parameters
        )

        return result

    def _generate_m2etis_ned(self):

        template = """
        //
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package m2etis.middleware;


import oversim.common.BaseApp;
import oversim.common.ITier;

//
// TODO auto-generated type
//
simple M2etisAdapter extends BaseApp
{
    parameters:
        @class(M2etisAdapter);
        $disable_overlays
        int packetSize @unit(B);
        int queueSize @unit(B);
        int channelCount;
        int downstream @unit(bps);
        int upstream @unit(bps);
        int headerSize @unit(B);
        int startRoot;
        int endRoot;
        int rendezvousNode;
        double stopAvg;
        double simulationResolution @unit(s);
        bool queueDisabled;
}

module M2etisPubSub like ITier
{
    gates:
        input udpIn;             // gate from the UDP layer
        output udpOut;           // gate to the UDP layer
        input from_lowerTier;    // gate from the lower tier
        input from_upperTier;    // gate from the upper tier
        output to_lowerTier;     // gate to the lower tier
        output to_upperTier;     // gate to the upper tier
        input trace_in;          // gate for trace file commands
        input tcpIn;             // gate from the TCP layer
        output tcpOut;           // gate to the TCP layer

    submodules:
        m2etis: M2etisAdapter;

    connections allowunconnected:
        from_lowerTier --> m2etis.from_lowerTier;
        to_lowerTier <-- m2etis.to_lowerTier;
        from_upperTier --> m2etis.from_upperTier;
        to_upperTier <-- m2etis.to_upperTier;
        udpIn --> m2etis.udpIn;
        udpOut <-- m2etis.udpOut;

}
        """

        disable_overlays = ""
        for i in range(0, self.number_of_channels):
            disable_overlays += "bool disableOverlay_" + str(i) + ";\n\t"

        template_prepared = Template(template)
        result = template_prepared.substitute(
            disable_overlays=disable_overlays
        )

        return result



