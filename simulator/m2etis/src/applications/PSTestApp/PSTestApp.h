/*
 Copyright (2016) Michael Baer, Daniel Bonrath, All rights reserved.

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

#ifndef PSTESTAPP_H_
#define PSTESTAPP_H_

#include <omnetpp.h>

#include <BaseApp.h>

namespace m2etis {

	class PSTestApp : public BaseApp {
		// module parameters
			simtime_t sendPeriod;     // we'll store the "sendPeriod" parameter here
			int numToSend;            // we'll store the "numToSend" parameter here
			int largestKey;           // we'll store the "largestKey" parameter here
			// statistics
			int numSent;              // number of packets sent
			int numReceived;          // number of packets received


			// our timer
			cMessage * timerMsg;
			bool subscribed_;

			// application routines
			void initializeApp(int stage);                 // called when the module is being created
			void finishApp();                              // called when the module is about to be destroyed
			void handleTimerEvent(cMessage * msg);          // called when we received a timer message
			void handleLowerMessage(cMessage * msg);			// called when we receive a message from the middleware

		public:
			PSTestApp() : timerMsg(NULL), subscribed_(false) {}

			~PSTestApp() {
				cancelAndDelete(timerMsg);
			}
	};

} /* namespace m2etis */

#endif /* PSTESTAPP_H_ */
