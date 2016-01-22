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

#ifndef __M2ETIS_M2ETISADAPTER_H__
#define __M2ETIS_M2ETISADAPTER_H__

#include <queue>

// include order matters beyond this point!!!
// This is needed to undefine a badly defined makro in omnet:
#undef ev
// No Omnet includes behind this line
	#include "m2etis/message/M2Message.h"
	#include "m2etis/pubsub/config/ChannelName.h"
	#include "m2etis/sim/OmNetMediator.h"
	#include "m2etis/sim/OmNetCallbackInterface.h"
	#include "m2etis/util/SegmentTree.h"
// reactivate OMNET MACRO
#define ev  (*cSimulation::getActiveEnvir())

#include "SimM2etisMessage_m.h"
#include "ApplicationCallback.h"

#include "BinaryValue.h"
#include "OverlayKey.h"

// Omnet Includes
#include "BaseApp.h"
#include "NodeHandle.h"

namespace m2etis {
namespace sim {
	class PubSubWrapper;
} /* namespace sim */

	/**
	 * \brief struct containing all necessary informations of a message
	 */
	typedef struct HelperStruct {
		std::string name;
		MessageType type;
		std::string key;
		pubsub::ChannelName chan;
		std::string owner;
		message::M2SimMessage::Ptr msg;
		int id;
		int size;
	} HelperStruct;

	class M2etisAdapter : public BaseApp, public sim::OmNetCallbackInterface {
		static std::map<std::string, std::string> KeyMapping;
		static std::map<int, bool> _overlays;
		static int _channelCount;

	public:
		/**
		 * \brief constructor
		 */
		M2etisAdapter();

		/**
		 * \brief destructor
		 */
		~M2etisAdapter();

		/**
		 * \brief method called by overlay to forward a message
		 *
		 * \param[in] key the key of the current node
		 * \param[in,out] msg the message being forwarded
		 * \param[in] nextHopNode the next node receiving this message
		 */
		void forward(OverlayKey * key, cPacket ** msg, ::NodeHandle * nextHopNode);

		/**
		 * \brief method called by network to receive a message from overlay
		 *
		 * \param[in] key the key of the sender
		 * \param[in] msg the message being delivered
		 */
		void deliver(OverlayKey & key, cMessage * msg);

		/**
		 * \brief called by overlay to update node list
		 *
		 * \param[in] node the node being updated
		 * \param[in] joined the flag whether the node joined or leaved
		 */
		void update(const ::NodeHandle & node, bool joined);

		/**
		 * \brief called when receiving a message from the upper tier (e.g. TupleFeeder)
		 *
		 * \param[in] msg the message being received
		 */
		void handleUpperMessage(cMessage * msg);

		/**
		 * \brief called for every "tick" of this module, updates every necessary part of the simulation
		 *
		 * \param[in] msg the timer message
		 */
		void handleTimerEvent(cMessage * msg);

		/**
		 * \brief called when module is ready for work
		 *
		 * \param[in] msg the message signaling the ready state
		 */
		void handleReadyMessage(CompReadyMessage * msg);

		/**
		 * \brief receives a message from the UDP channel in simulator
		 *
		 * \param[in] msg the message being received
		 */
		void handleUDPMessage(cMessage * msg);

		/**
		 * \brief sends the given message through the BaseApp to the simulated UDP channel
		 *
		 * \param[in] destAddr the address of the node receiving this message
		 * \param[in] msg the message being sent to the channel
		 * \param[in] delay the delay for this message
		 */
		void sendMessageToUDP(const TransportAddress & destAddr, cPacket * msg, simtime_t delay = SIMTIME_ZERO) { BaseApp::sendMessageToUDP(destAddr, msg, delay); }

		/**
		 * \brief called when module is initialized
		 *
		 * \param[in] stage the state of the module
		 */
		void initializeApp(int stage);

		/**
		 * \brief called when simulation is finished
		 */
		void finishApp();

		/**
		 * \brief received message from OmNetMediator to send them into the network
		 *
		 * \param[in] type message type of the message being sent
		 * \param[in] to key of the receiver
		 * \param[in] payload message being sent
		 * \param[in] hint NodeHandle
		 * \param[in] size the size of the serialized message for metric
		 */
		void send(const message::ActionType type, const std::string & to, const message::M2SimMessage::Ptr payload, net::NodeHandle<net::NetworkType<net::OMNET> >::Ptr_const hint, unsigned int size);

		/**
		 * \brief returns the SHA1 key for the given topic
		 *
		 * \param[in] channel_name the topic for which the SHA1 key should be generated
		 */
		std::string getSHA1(int channel_name);

		/**
		 * \brief returns the current simulation time, called by OmNetClock
		 */
		long getSimTime() const;

		// Lost messages
		static double avgMsgLoss;
		static int lostMsgs;
		static int nodesMissingMsgs;

		static util::SegmentTree<int> _lostMsgIDs;

	private:
		/**
		 * \brief sends as many message to the network as bandwith allows
		 */
		void flushDownBuffer();

		/**
		 * \brief receives as many messages from the network as bandwith allows
		 */
		void flushInputBuffer();

		/**
		 * \brief checks whether a message with the given size can be sent or not
		 *
		 * \param[in] size the size of the message which should be sent
		 */
		bool canSend(int size);

		/**
		 * \brief adds the given size to the list of currently transported message
		 *
		 * \param[in] size of the message being added
		 */
		void addSendMessage(int size);

		/**
		 * \brief removes every tick all message sizes being send out
		 */
		void purgeOutChannel();

		/**
		 * \brief checks whether a message with the given size can be received or not
		 *
		 * \param[in] size the size of the message which should be received
		 */
		bool canReceive(int size);

		/**
		 * \brief adds the given size to the list of currently received message
		 *
		 * \param[in] size of the message being added
		 */
		void addReceiveMessage(int size);

		/**
		 * \brief removes every tick all message sizes being received
		 */
		void purgeInChannel();

		cMessage * timerMsg;
		sim::PubSubWrapper * metisfrontend_;
		sim::OmNetMediator * metisbackend_;
		ApplicationCallback * upcall_;
		std::string key_;
		std::string name_;
		int port_;
		std::queue<HelperStruct *> downbuffer_;
		bool m2etis_initialized;

		int _packetSize;
		int _headerSize;

		int _controlMsgsS;
		int _controlMsgsR;

		util::SegmentTree<int> _msgMap;

		int _duplicates;

		int _downstream;
		int _upstream;

		std::list<std::pair<simtime_t, int> > _outChannel;
		std::list<std::pair<simtime_t, int> > _inChannel;

		std::queue<SimM2etisMessage *> _inputChannel;
		int _queueSize;
		int _currentQueueSize;

		static long _msgCount;
		static long _msgSize;
		static long _minSize;
		static long _maxSize;
		static long _pubCount;

		double _stopAvg;
		double _simulationResolution;
		bool _queueDisabled;
	};

} /* namespace m2etis */

#endif /* __M2ETIS_M2ETISADAPTER_H__ */

/**
 *  @}
 */
