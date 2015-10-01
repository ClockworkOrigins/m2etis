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
        double numToSend_0;             // how many packets will we send at the same time?
        double numToSend_1;             // how many packets will we send at the same time?
        double numToSend_2;             // how many packets will we send at the same time?
        double numToSend_3;             // how many packets will we send at the same time?
        double numToSend_4;             // how many packets will we send at the same time?
        double numToSend_5;             // how many packets will we send at the same time?
        double numToSend_6;             // how many packets will we send at the same time?
        double numToSend_7;             // how many packets will we send at the same time?
        double numToSend_8;             // how many packets will we send at the same time?
        double numToSend_9;             // how many packets will we send at the same time?
        double numToSend_10;             // how many packets will we send at the same time?
        double numToSend_11;             // how many packets will we send at the same time?
        double numToSend_12;             // how many packets will we send at the same time?
        double numToSend_13;             // how many packets will we send at the same time?
        double numToSend_14;             // how many packets will we send at the same time?
        double numToSend_15;             // how many packets will we send at the same time?
        double numToSend_16;             // how many packets will we send at the same time?
        int largestKey;            // largest key we can pick
        int numSubs;
        int numPubs;
        int numPubSubs;
        int numRend;
        int channelCount;
        int burstAmount_0;
        int burstAmount_1;
        int burstAmount_2;
        int burstAmount_3;
        int burstAmount_4;
        int burstAmount_5;
        int burstAmount_6;
        int burstAmount_7;
        int burstAmount_8;
        int burstAmount_9;
        int burstAmount_10;
        int burstAmount_11;
        int burstAmount_12;
        int burstAmount_13;
        int burstAmount_14;
        int burstAmount_15;
        int burstAmount_16;
        int burstFrequency_0 @unit(s);
        int burstFrequency_1 @unit(s);
        int burstFrequency_2 @unit(s);
        int burstFrequency_3 @unit(s);
        int burstFrequency_4 @unit(s);
        int burstFrequency_5 @unit(s);
        int burstFrequency_6 @unit(s);
        int burstFrequency_7 @unit(s);
        int burstFrequency_8 @unit(s);
        int burstFrequency_9 @unit(s);
        int burstFrequency_10 @unit(s);
        int burstFrequency_11 @unit(s);
        int burstFrequency_12 @unit(s);
        int burstFrequency_13 @unit(s);
        int burstFrequency_14 @unit(s);
        int burstFrequency_15 @unit(s);
        int burstFrequency_16 @unit(s);
        int burstDuration_0 @unit(s);
        int burstDuration_1 @unit(s);
        int burstDuration_2 @unit(s);
        int burstDuration_3 @unit(s);
        int burstDuration_4 @unit(s);
        int burstDuration_5 @unit(s);
        int burstDuration_6 @unit(s);
        int burstDuration_7 @unit(s);
        int burstDuration_8 @unit(s);
        int burstDuration_9 @unit(s);
        int burstDuration_10 @unit(s);
        int burstDuration_11 @unit(s);
        int burstDuration_12 @unit(s);
        int burstDuration_13 @unit(s);
        int burstDuration_14 @unit(s);
        int burstDuration_15 @unit(s);
        int burstDuration_16 @unit(s);
        double dropChance_0;
        double dropChance_1;
        double dropChance_2;
        double dropChance_3;
        double dropChance_4;
        double dropChance_5;
        double dropChance_6;
        double dropChance_7;
        double dropChance_8;
        double dropChance_9;
        double dropChance_10;
        double dropChance_11;
        double dropChance_12;
        double dropChance_13;
        double dropChance_14;
        double dropChance_15;
        double dropChance_16;
        bool compensateDrop_0;
        bool compensateDrop_1;
        bool compensateDrop_2;
        bool compensateDrop_3;
        bool compensateDrop_4;
        bool compensateDrop_5;
        bool compensateDrop_6;
        bool compensateDrop_7;
        bool compensateDrop_8;
        bool compensateDrop_9;
        bool compensateDrop_10;
        bool compensateDrop_11;
        bool compensateDrop_12;
        bool compensateDrop_13;
        bool compensateDrop_14;
        bool compensateDrop_15;
        bool compensateDrop_16;
        double fluctuation_0;
        double fluctuation_1;
        double fluctuation_2;
        double fluctuation_3;
        double fluctuation_4;
        double fluctuation_5;
        double fluctuation_6;
        double fluctuation_7;
        double fluctuation_8;
        double fluctuation_9;
        double fluctuation_10;
        double fluctuation_11;
        double fluctuation_12;
        double fluctuation_13;
        double fluctuation_14;
        double fluctuation_15;
        double fluctuation_16;
        double stopAvg;
        double stopLoss;
        int waitForSubscribe @unit(s);
        int waitForPublish @unit(s);
        double minStayDuration @unit(s);
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
