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

/**
 * \addtogroup simulator
 * @ {
 */

#ifndef __M2ETIS_TUPLEFEEDER_H__
#define __M2ETIS_TUPLEFEEDER_H__

#include "m2etis/util/SegmentTree.h"

#include <omnetpp.h>

#include <BaseApp.h>

namespace m2etis {

	/**
	 * \brief struct containing all informations about a channel being used in simulator
	 */
	typedef struct ChannelInfoStruct {
		int channel;
		double numToSend;
		int tickToSend;
		int numSubs = 0;

		// Time from sender to receiver
		double minMsgTime = -1.0;
		double maxMsgTime = 0.0;
		double allTimes = 0.0;
		int msgCounter = 0;
		double msgAvgTime = 0.0;

		// Time from sender to all receivers
		double minMsgCTime = -1.0;
		double maxMsgCTime = 0.0;
		double allCTimes = 0.0;
		int msgCCounter = 0;
		double msgCAvgTime = 0.0;

		// Time from subscribe to first publish
		double minFCTime = -1.0;
		double maxFCTime = 0.0;
		double allFCTimes = 0.0;
		int FCCounter = 0;
		double FCAvgTime = 0.0;

		// Duplicated messages
		int duplicateMsgs = 0;
		bool isOrdered = true;

		// Burst mode
		int burstFrequency = 0;
		int burstDuration = 0;
		int burstAmount = 0;

		double dropChance = 0;
		bool compensateDrop = false;

		// Fluctuation of Subscribers
		int fluctuation = 0;
		int missingFluctuation = 0;
		int fluctuationID = 0;
	} ChannelInfoStruct;

	class TupleFeeder : public BaseApp {
		enum AppState {
			NONE,		//!< No state
			SUBSCRIBER,	//!< Node only subscribes
			PUBLISHER,	//!< Node only publishes
			PUBSUBER,	//!< Node subscribes and publishes
			RENDEVOUZ	//!< Node is only a Rendesvouz point neither subscribing  nor publishing
		};

	public:
		static int numSubs;
		static int numPubs;
		static int numPubSubs;
		static int numRend;
		static int numPubsR;

		static bool rootNodeInitialized;

		static int messageID;

		// Time from sender to receiver
		static double minMsgTime;
		static double maxMsgTime;
		static double allTimes;
		static int msgCounter;
		static double msgAvgTime;

		// Time from sender to all receivers
		static double minMsgCTime;
		static double maxMsgCTime;
		static double allCTimes;
		static int msgCCounter;
		static double msgCAvgTime;

		// Time from subscribe to first publish
		static double minFCTime;
		static double maxFCTime;
		static double allFCTimes;
		static int FCCounter;
		static double FCAvgTime;

		// Duplicated messages
		static int duplicateMsgs;

		static double startTimeNode;

		struct AliveMessage {
			int id;				// Msg ID
			simtime_t sendTime;	// Sent Time
			int expectedRecvs;	// Number of nodes that shall receive it
			int missingRecvs;	// number of nodes thatnot yet received it
		};
		//				id				sendTime   expected receivers
		static std::map<int, AliveMessage> _messages;

		static double lossTimeout;

		std::map<int, int> _lastIDs;

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

		static boost::posix_time::ptime startTime;
		static boost::posix_time::ptime endTime;
		static std::map<int, ChannelInfoStruct *> _channels;
		static int id;
		static bool stopAll;
		static double minStayDuration;

		/**
		 * \brief called when the module is being created
		 *
		 * \param[in] stage the stage in which the module currently is
		 */
		void initializeApp(int stage);

		/**
		 * \brief called when the module is about to be destroyed
		 */
		void finishApp();

		/**
		 * \brief called when we received a timer message
		 *
		 * \param[in] msg the timer message being received
		 */
		void handleTimerEvent(cMessage * msg);

		/**
		 * \brief called when we receive a message from the M2etisAdapter
		 *
		 * \param[in] msg message being received by M2etisAdapter
		 */
		void handleLowerMessage(cMessage * msg);

		/**
		 * \brief called when module is ready to start working
		 *
		 * \param[in] msg the message signaling the ready state
		 */
		void handleReadyMessage(CompReadyMessage * msg);

	public:
		/**
		 * \brief constructor
		 */
		TupleFeeder() : _lastIDs(), timerMsg(NULL), subscribed_(false), _state(0), _startSub(), _receivedMsgs(), _ticks(), _burst(), _subscribedOnChannel(), m2etis_initialized(false), _orderMap(), _stopAvg(1000000), _stopLoss(100.0), _stopped(false), _joinTime() {
		}

		/**
		 * \brief destructor
		 */
		~TupleFeeder() {
			cancelAndDelete(timerMsg);
		}

	private:
		unsigned short _state;
		std::map<int, simtime_t> _startSub;
		util::SegmentTree<int> _receivedMsgs;

		static int _channelCount;
		int _ticks;
		std::map<int, bool> _burst;
		std::map<int, bool> _subscribedOnChannel;
		bool m2etis_initialized;

		/**
		 * \brief struct containing informations about the ordering of a channel
		 */
		typedef struct orderMap {
			long _inOrder = 0;
			long _outOrder = 0;
			long _overAll = 0;
		} orderMap;

		std::map<int, orderMap> _orderMap;

		typedef struct sendStruct {
			double sendPeriod = 0.0;
			cMessage * timerMsg = nullptr;
			int ticks = 0;
			int secTicks = 0;
			double ticksPerSec = 0;
		} sendStruct;

		std::map<int, sendStruct *> _sendChannels;

		int _id;

		double _stopAvg;
		double _stopLoss;
		bool _stopped;
		double _waitForSubscribe;
		double _waitForPublish;
		cMessage * _subscribeMsg = nullptr;
		cMessage * waitForInitMsg = nullptr;
		bool _initialized = false;
		double _joinTime;
		double _runTime = -1.0;

		/**
		 * \brief handles subscribe part of each tick
		 */
		bool handleSubscribe(bool sec, std::pair<int, sendStruct *> s);

		double getCurrentLoss();
	};

} /* namespace m2etis */

#endif /* __M2ETIS_TUPLEFEEDER_H__ */

/**
 *  @}
 */
