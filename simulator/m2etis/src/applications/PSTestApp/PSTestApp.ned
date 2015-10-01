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

package m2etis.applications.PSTestApp;

import oversim.common.BaseApp;
import oversim.common.ITier;

simple PSTestApp extends BaseApp
{
    parameters:
        @class(PSTestApp);
        int sendPeriod @unit(s);   // how much time to wait between sending packets
        int numToSend;             // how many packets will we send at the same time?
        int largestKey;            // largest key we can pick
}


module PSTestAppModules like ITier
{
    parameters:
        @display("i=block/segm");
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
        psTestApp: PSTestApp;

    connections allowunconnected:
        from_lowerTier --> psTestApp.from_lowerTier;
        to_lowerTier <-- psTestApp.to_lowerTier;
        trace_in --> psTestApp.trace_in;
        udpIn --> psTestApp.udpIn;
        udpOut <-- psTestApp.udpOut;
}