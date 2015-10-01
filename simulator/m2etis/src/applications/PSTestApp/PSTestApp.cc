/**
 Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <string>

#include "m2etis/pubsub/config/ChannelName.h"

#include "UnderlayConfigurator.h"
#include "GlobalStatistics.h"

#include "PSTestApp.h"
#include <middleware/SimM2etisMessage_m.h>

namespace m2etis {

	Define_Module(PSTestApp);

	void PSTestApp::initializeApp(int stage) {
		if (stage == MIN_STAGE_APP) {
		// copy the module parameter values to our own variables
			sendPeriod = par("sendPeriod");
			numToSend = par("numToSend");
			largestKey = par("largestKey");
			// initialize our statistics variables
			numSent = 0;
			numReceived = 0;

			// tell the GUI to display our variables
			WATCH(numSent);
			WATCH(numReceived);

			// start our timer!
			timerMsg = new cMessage("PSTestApp Timer");
			scheduleAt(simTime() + sendPeriod, timerMsg);
		}
	}

	void PSTestApp::finishApp() {
		// finish() is usually used to save the module's statistics.
		// We'll use globalStatistics->addStdDev(), which will calculate min, max, mean and deviation values.
		// The first parameter is a name for the value, you can use any name you like (use a name you can find quickly!).
		// In the end, the simulator will mix together all values, from all nodes, with the same name.

		//unsubscribe
		SimM2etisMessage * unsub = new SimM2etisMessage();
		sendMessageToLowerTier(unsub);

		globalStatistics->addStdDev("MyApplication: Sent packets", numSent);
		globalStatistics->addStdDev("MyApplication: Received packets", numReceived);
	}

	void PSTestApp::handleTimerEvent(cMessage * msg) {
		// is this our timer?
		if (msg == timerMsg) {
			// reschedule our message
			scheduleAt(simTime() + sendPeriod, timerMsg);

			// if the simulator is still busy creating the network,
			// let's wait a bit longer
			if (underlayConfigurator->isInInitPhase()) {
				return;
			}

			if (!subscribed_) {
				SimM2etisMessage * sub = new SimM2etisMessage("OTHER:SubscribeMessage");
				sub->setType(O_SUBSCRIBE);
				sub->setChannelName(0);
				message::M2SimMessage::Ptr m(new message::M2SimMessage());
				sub->setIntmessage(m);
				sendMessageToLowerTier(sub); // send it to m2etis

				subscribed_ = true;
			}

			for (int i = 0; i < numToSend; ++i) {
				SimM2etisMessage * publish = new SimM2etisMessage("OTHER:Publish Message");
				publish->setType(O_PUBLISH);
				publish->setChannelName(0);
				message::M2SimMessage::Ptr m(new message::M2SimMessage());
				publish->setIntmessage(m);

				numSent++; // update statistics

				EV << thisNode.getIp() << ": Sending " << publish->getType() << " packet to channel" << publish->getChannelName()
						<< "!" << std::endl;

				sendMessageToLowerTier(publish); // send it to m2etis
			}
		} else {
			// unknown message types are discarded
			delete msg;
		}
	}

	void PSTestApp::handleLowerMessage(cMessage * msg) {
		SimM2etisMessage * myMsg = dynamic_cast<SimM2etisMessage *>(msg);

		if (myMsg == NULL) {
			delete msg; // type unknown!
			return;
		}

		std::cerr << "\nGot packed from: " << myMsg->getKey() << ", type:" << myMsg->getType() << std::endl;

		EV << "\n" << thisNode.getIp() << ": Got packet from "
					   << myMsg->getKey() << ", type:"
					   << myMsg->getType()
					   << std::endl;

		delete msg;
	}

} /* namespace m2etis */
