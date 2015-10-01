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
        bool disableOverlay_0;
        bool disableOverlay_1;
        bool disableOverlay_2;
        bool disableOverlay_3;
        bool disableOverlay_4;
        bool disableOverlay_5;
        bool disableOverlay_6;
        bool disableOverlay_7;
        bool disableOverlay_8;
        bool disableOverlay_9;
        bool disableOverlay_10;
        bool disableOverlay_11;
        bool disableOverlay_12;
        bool disableOverlay_13;
        bool disableOverlay_14;
        bool disableOverlay_15;
        bool disableOverlay_16;
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
