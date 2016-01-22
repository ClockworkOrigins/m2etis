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

#include "M2etisAdapter.h"

#undef ev
	#include "m2etis/sim/i6eRandom.h"
	#include "m2etis/message/InternalMessage.h"
	#include "m2etis/net/NetworkFactory.h"
	#include "m2etis/pubsub/PubSubSystemEnvironment.h"
	#include "m2etis/sim/PubSubWrapper.h"
#define ev  (*cSimulation::getActiveEnvir())

#include "TupleFeeder.h"

#include "GlobalStatistics.h"

namespace m2etis {

	Define_Module(M2etisAdapter);

	std::map<std::string, std::string> M2etisAdapter::KeyMapping = std::map<std::string, std::string>();
	std::map<int, bool> M2etisAdapter::_overlays = std::map<int, bool>();
	int M2etisAdapter::_channelCount = 0;

	long M2etisAdapter::_msgCount = 0;
	long M2etisAdapter::_msgSize = 0;
	long M2etisAdapter::_minSize = 0;
	long M2etisAdapter::_maxSize = 0;
	long M2etisAdapter::_pubCount = 0;

	double M2etisAdapter::avgMsgLoss = 0;
	int M2etisAdapter::lostMsgs = 0;
	int M2etisAdapter::nodesMissingMsgs = 0;

	util::SegmentTree<int> M2etisAdapter::_lostMsgIDs = util::SegmentTree<int>();

	M2etisAdapter::M2etisAdapter() : metisfrontend_(NULL), metisbackend_(NULL), upcall_(new ApplicationCallback(this)), key_(thisNode.getKey().toString()), name_(thisNode.getIp().str()), port_(thisNode.getPort()), downbuffer_(), m2etis_initialized(false), _packetSize(0), _headerSize(0), _controlMsgsS(0), _controlMsgsR(0), _msgMap(), _duplicates(0), _downstream(0), _upstream(0), _outChannel(), _inChannel(), _inputChannel(), _queueSize(), _currentQueueSize(), _simulationResolution(), _queueDisabled(false) {
	}

	M2etisAdapter::~M2etisAdapter() {
		delete upcall_;
		delete metisfrontend_;
	}

	void M2etisAdapter::forward(OverlayKey * key, cPacket ** msg, ::NodeHandle * nextHopNode) {
		std::stringstream ss;
		ss << nextHopNode->getIp().str() << ":" << nextHopNode->getPort();
		net::NetworkType<net::OMNET>::Key tmp(ss.str());
		net::NodeHandle<net::NetworkType<net::OMNET> >::Ptr nh(new net::NodeHandle<net::NetworkType<net::OMNET> >(tmp, nextHopNode->getIp().str(), nextHopNode->getPort()));

		SimM2etisMessage * m = dynamic_cast<SimM2etisMessage *>(*msg);

		// nothing to be done:
		if (m == NULL) {
			return;
		}

		std::string returnkey(key->toString());

		if (metisbackend_->forward(returnkey, m->getIntmessage(), nh)) {
			delete *msg;
			*msg = NULL;
		}
	}

	void M2etisAdapter::deliver(OverlayKey & key, cMessage * msg) {
		SimM2etisMessage * m = dynamic_cast<SimM2etisMessage *>(msg);
		if (m->getType() == O_CONTROL) {
			_controlMsgsR++;
		}

		if (m->getType() == O_PUBLISH || m->getType() == O_NOTIFY) {
			if (!_msgMap.contains(m->getIntmessage()->payload->_simID)) {
				_msgMap.insert(m->getIntmessage()->payload->_simID);
			} else {
				_duplicates++;
			}
		}

		if (canReceive(m->getBitLength())) {
			addReceiveMessage(m->getBitLength());
			metisbackend_->deliver(metisfrontend_->transformToNetworkMessage(m->getIntmessage(), static_cast<pubsub::ChannelName>(m->getChannelName())));
			delete m;
		} else {
			_inputChannel.push(m);
		}
	}

	void M2etisAdapter::update(const ::NodeHandle & node, bool joined) {
		std::stringstream ss;
		ss << node.getIp().str() << ":" << node.getPort();
		net::NetworkType<net::OMNET>::Key tmp(ss.str());
		net::NodeHandle<net::NetworkType<net::OMNET> >::Ptr_const mnh(new net::NodeHandle<net::NetworkType<net::OMNET> >(tmp, node.getIp().str(), node.getPort()));
		metisbackend_->update(ss.str(), mnh, joined);
	}

	void M2etisAdapter::handleUDPMessage(cMessage * msg) {
		SimM2etisMessage * m = dynamic_cast<SimM2etisMessage *>(msg);
		if (m->getType() == O_CONTROL) {
			_controlMsgsR++;
		}

		if (m->getType() == O_PUBLISH || m->getType() == O_NOTIFY) {
			if (!_msgMap.contains(m->getIntmessage()->payload->_simID)) {
				_msgMap.insert(m->getIntmessage()->payload->_simID);
			} else {
				_duplicates++;
			}
		}

		if (canReceive(m->getBitLength())) {
			addReceiveMessage(m->getBitLength());
			metisbackend_->deliver(metisfrontend_->transformToNetworkMessage(m->getIntmessage(), static_cast<pubsub::ChannelName>(m->getChannelName())));
			delete m;
		} else {
			_inputChannel.push(m);
		}
	}

	void M2etisAdapter::handleUpperMessage(cMessage * msg) {
		SimM2etisMessage * m = dynamic_cast<SimM2etisMessage *>(msg);

		assert(m->getChannelName() >= 0 && m->getChannelName() < _channelCount);

		SimulationEventType v;
		v._simID = m->getId();
		v._simChannel = m->getChannelName();

		message::M2SimMessage::Ptr newMsg = metisfrontend_->createMessage(static_cast<pubsub::ChannelName>(m->getChannelName()), v);

		switch(m->getType()) {
			case ::O_PUBLISH: {
				metisfrontend_->publish(static_cast<pubsub::ChannelName>(m->getChannelName()), newMsg);
				break;
			}
			case ::O_SUBSCRIBE: {
				metisfrontend_->subscribe(static_cast<pubsub::ChannelName>(m->getChannelName()), *upcall_);
				break;
			}
			case ::O_UNSUBSCRIBE: {
				metisfrontend_->unsubscribe(static_cast<pubsub::ChannelName>(m->getChannelName()));
				break;
			}
			default: {
				assert(false);
				break;
			}
		}

		delete m;
	}

	void M2etisAdapter::handleTimerEvent(cMessage * msg) {
		if (TupleFeeder::startTimeNode > 10.0) {
			if (TupleFeeder::stopAll) {
				return;
			}
		}
		scheduleAt(simTime() + _simulationResolution, timerMsg);
		purgeOutChannel();
		purgeInChannel();
		flushDownBuffer();
		flushInputBuffer();

		metisfrontend_->Update();
	}

	void M2etisAdapter::initializeApp(int stage) {
		if (stage != MIN_STAGE_APP) {
			return;
		}

		WATCH(_controlMsgsS);
		WATCH(_controlMsgsR);
		WATCH(_duplicates);

		bindToPort(2000);

		if (_channelCount == 0) {
			_channelCount = par("channelCount");

			for (int i = 0; i < _channelCount; ++i) {
				_overlays[i] = par((std::string("disableOverlay_") + boost::lexical_cast<std::string>(i)).c_str());
			}
		}

		_downstream = par("downstream");
		_upstream = par("upstream");
		_headerSize = par("headerSize");
		_headerSize *= 8;
		_stopAvg = par("stopAvg");
		_queueDisabled = par("queueDisabled");
	}

	void M2etisAdapter::finishApp() {
		cancelAndDelete(timerMsg);

		globalStatistics->addStdDev("TupleFeeder: Control Messages Send", _controlMsgsS);
		globalStatistics->addStdDev("TupleFeeder: Control Messages Received", _controlMsgsR);
		globalStatistics->addStdDev("TupleFeeder: Duplicates at each link", _duplicates);

		if (_msgCount > 0) {
			globalStatistics->addStdDev("TupleFeeder: Message Size - Avg", _msgSize / _msgCount);
		} else {
			globalStatistics->addStdDev("TupleFeeder: Message Size - Avg", -1);
		}

		globalStatistics->addStdDev("TupleFeeder: Message Size - Min", _minSize);
		globalStatistics->addStdDev("TupleFeeder: Message Size - Max", _maxSize);

		if (TupleFeeder::messageID > 0) {
			globalStatistics->addStdDev("TupleFeeder: Message Lost For All Nodes", (nodesMissingMsgs * 100.0) / TupleFeeder::messageID); // Percentage of messages not being received at all nodes
		} else {
			globalStatistics->addStdDev("TupleFeeder: Message Lost For All Nodes", 0); // Percentage of messages not being received at all nodes
		}

		if (_pubCount > 0) {
			globalStatistics->addStdDev("TupleFeeder: Message Lost of all messages", (lostMsgs * 100.0) / _pubCount); // Percentage of messages not being received at all nodes
		} else {
			globalStatistics->addStdDev("TupleFeeder: Message Lost of all messages", 0); // Percentage of messages not being received at all nodes
		}
	}

	void M2etisAdapter::send(const message::ActionType type, const std::string & to, const message::M2SimMessage::Ptr msg, net::NodeHandle<net::NetworkType<net::OMNET> >::Ptr_const hint, unsigned int size) {
		MessageType mtype;
		std::string name = "";

		HelperStruct * h = new HelperStruct();

		switch(type & ~message::MSG_TYPE_MASK) {
			case message::PUBLISH: {
				mtype = O_PUBLISH;
				name = "Publish Message";
				_pubCount++;
				break;
			}
			case message::SUBSCRIBE: {
				mtype = O_SUBSCRIBE;
				name = "Subscribe Message";

				break;
			}
			case message::UNSUBSCRIBE: {
				mtype = O_UNSUBSCRIBE;
				name = "Unsubscribe Message";
				break;
			}
			case message::CONTROL: {
				mtype = O_CONTROL;
				name = "Control Message";
				_controlMsgsS++;
				break;
			}
			case message::NOTIFY: {
				mtype = O_NOTIFY;
				name = "Notify Message";
				_pubCount++;
				break;
			}
			case message::JOIN: {
				mtype = O_JOIN;
				name = "Join Message";

				if (TupleFeeder::_channels.find(static_cast<pubsub::ChannelName>(msg->payload->_simChannel)) == TupleFeeder::_channels.end()) {
					delete h;
					return;
				}
				break;
			}
			case message::STATE: {
				mtype = O_STATE;
				name = "State Message";

				if (TupleFeeder::_channels.find(static_cast<pubsub::ChannelName>(msg->payload->_simChannel)) == TupleFeeder::_channels.end()) {
					delete h;
					return;
				}
				break;
			}
			case message::LEAVE: {
				mtype = O_LEAVE;
				name = "Leave Message";

				if (TupleFeeder::_channels.find(static_cast<pubsub::ChannelName>(msg->payload->_simChannel)) == TupleFeeder::_channels.end()) {
					delete h;
					return;
				}
				break;
			}
			default: {
				assert(false);
				mtype = O_NONE;
				name = "Undefined Message Type";
				break;
			}
		}

		h->name = name;
		h->type = mtype;

		h->key = to;

		h->chan = static_cast<pubsub::ChannelName>(msg->payload->_simChannel);
		h->id = msg->payload->_simID;

		h->msg = msg;

		h->owner = key_;

		++_msgCount;
		_msgSize += size;

		if (_minSize > size || _minSize == 0) {
			_minSize = size;
		}

		if (_maxSize < size) {
			_maxSize = size;
		}

		if (h->type == O_PUBLISH || h->type == O_NOTIFY) {
			h->size = (size * 8) + _headerSize + _packetSize;
		} else {
			h->size = (size * 8) + _headerSize;
		}

		int kk = util::Random::rand(100);
		if (kk < TupleFeeder::_channels[h->chan]->dropChance) {
			// counts every message being dropped and causes reduced measurement for all nodes metric
			if (h->id != -1) {
				if (!_lostMsgIDs.contains(h->id)) {
					_lostMsgIDs.insert(h->id);

					nodesMissingMsgs++;
				}

				lostMsgs++;
			}

			delete h;
			return;
		} else {
			if (canSend(h->size) && downbuffer_.empty()) {
				SimM2etisMessage * newMsg = new SimM2etisMessage(h->name.c_str());
				newMsg->setType(h->type);
				newMsg->setChannelName(h->chan);
				newMsg->setIntmessage(h->msg);

				newMsg->setBitLength(h->size);

				addSendMessage(newMsg->getBitLength());

				if (_overlays[h->chan] || h->type == O_NOTIFY) {
					std::string ip = h->key.substr(0, h->key.find(":"));
					int port = std::atoi(h->key.substr(h->key.find(":") + 1, h->key.size()).c_str());
					sendMessageToUDP(TransportAddress(IPvXAddress(ip.c_str()), port), newMsg);
				} else {
					callRoute(KeyMapping[h->key], newMsg);
				}

				delete h;
				return;
			}
		}

		if (_queueSize - _currentQueueSize >= h->size || _queueDisabled) {
			downbuffer_.push(h);
			_currentQueueSize += h->size;
		} else {
			flushDownBuffer();

			// counts every message being dropped and causes reduced measurement for all nodes metric
			if (downbuffer_.front()->id != -1) {
				if (!_lostMsgIDs.contains(downbuffer_.front()->id)) {
					_lostMsgIDs.insert(downbuffer_.front()->id);

					nodesMissingMsgs++;
				}

				lostMsgs++;
			}

			_currentQueueSize -= downbuffer_.front()->size;

			delete downbuffer_.front();

			downbuffer_.pop();

			downbuffer_.push(h);
			_currentQueueSize += h->size;
		}
	}

	void M2etisAdapter::flushDownBuffer() {
		// One Iteration on whole Buffer
		while (!downbuffer_.empty()) {
			HelperStruct * m = downbuffer_.front();

			if (canSend(m->size)) {
				SimM2etisMessage * newMsg = new SimM2etisMessage(m->name.c_str());
				newMsg->setType(m->type);
				newMsg->setChannelName(m->chan);
				newMsg->setIntmessage(m->msg);

				newMsg->setBitLength(m->size);

				addSendMessage(newMsg->getBitLength());

				if (_overlays[m->chan] || m->type == O_NOTIFY) {
					std::string ip = m->key.substr(0, m->key.find(":"));
					int port = std::atoi(m->key.substr(m->key.find(":") + 1, m->key.size()).c_str());
					sendMessageToUDP(TransportAddress(IPvXAddress(ip.c_str()), port), newMsg);
				} else {
					callRoute(KeyMapping[m->key], newMsg);
				}
				_currentQueueSize -= m->size;

				downbuffer_.pop();

				delete m;
			} else {
				break;
			}
		}
	}

	void M2etisAdapter::flushInputBuffer() {
		// One Iteration on whole Buffer
		while (!_inputChannel.empty()) {
			SimM2etisMessage * m = _inputChannel.front();

			if (canReceive(m->getBitLength())) {
				addReceiveMessage(m->getBitLength());
				metisbackend_->deliver(metisfrontend_->transformToNetworkMessage(m->getIntmessage(), static_cast<pubsub::ChannelName>(m->getChannelName())));
				_inputChannel.pop();
				delete m;
			} else {
				break;
			}
		}
	}

	void M2etisAdapter::handleReadyMessage(CompReadyMessage * msg) {
		// if m2etis has been initialized or the OverlayKey is not generated yet, return
		if (m2etis_initialized || overlay->getThisNode().getKey().toString().compare("<unspec>") == 0) {
			delete msg;
			return;
		}

		_packetSize = par("packetSize");
		_packetSize *= 8;
		_queueSize = par("queueSize");
		_queueSize *= 8;

		int startRoot = par("startRoot");
		int endRoot = par("endRoot");
		int rendezvousNode = par("rendezvousNode");

		_simulationResolution = par("simulationResolution");

		name_ = thisNode.getIp().str();
		port_ = thisNode.getPort();
		key_ = overlay->getThisNode().getKey().toString();

		std::stringstream ss;
		ss << name_ << ":" << port_;

		KeyMapping[ss.str()] = key_;

		std::vector<std::string> roots;

		for (int i = startRoot; i <= endRoot; ++i) {
			roots.push_back(std::string("1.0.0.") + boost::lexical_cast<std::string>(i));
		}

		metisfrontend_ = new sim::PubSubWrapper(this, name_, port_, std::string("1.0.0.") + boost::lexical_cast<std::string>(rendezvousNode), port_, roots);
		metisbackend_ = dynamic_cast<sim::OmNetMediator *>(metisfrontend_->pssi()->_factory->createNetworkController(net::NetworkType<net::OMNET>())->network_);
		metisbackend_->registerWrapper(metisfrontend_);

		EV << ">>>>> initialized m2etis Node with IP " << name_ << " Port: " << port_ << " and Key " << key_ << std::endl;

		timerMsg = new cMessage("M2etisAdapter Timer");
		scheduleAt(simTime() + _simulationResolution, timerMsg);

		m2etis_initialized = true;

	    delete msg;
	}

	std::string M2etisAdapter::getSHA1(int channel_name) {
		BinaryValue b_value(channel_name);
		OverlayKey topic_key = OverlayKey::sha1(b_value);
		return topic_key.toString();
	}

	long M2etisAdapter::getSimTime() const {
		return static_cast<long>(simTime().dbl() * 1000000.0); // simTime() returns time in seconds, clock needs microseconds
	}

	bool M2etisAdapter::canSend(int size) {
		int usedSize = 0;

		for (std::pair<simtime_t, int> p : _outChannel) {
			usedSize += p.second;
		}

		if (usedSize == 0 && size > _upstream) {
			return true;
		}
		return size <= _upstream - usedSize;
	}

	void M2etisAdapter::addSendMessage(int size) {
		_outChannel.push_back(std::make_pair(simTime() + (size - 1) / _upstream, size));
	}

	void M2etisAdapter::purgeOutChannel() {
		while (!_outChannel.empty() && (simTime() - _outChannel.front().first).dbl() >= 1.0) {
			_outChannel.pop_front();
		}
	}

	bool M2etisAdapter::canReceive(int size) {
		int usedSize = 0;

		for (std::pair<simtime_t, int> p : _inChannel) {
			usedSize += p.second;
		}

		if (usedSize == 0 && size > _downstream) {
			return true;
		}

		return size <= _downstream - usedSize;
	}

	void M2etisAdapter::addReceiveMessage(int size) {
		_inChannel.push_back(std::make_pair(simTime() + (size - 1) / _downstream, size));
	}

	void M2etisAdapter::purgeInChannel() {
		while (!_inChannel.empty() && (simTime() - _inChannel.front().first).dbl() >= 1.0) {
			_inChannel.pop_front();
		}
	}

} /* namespace m2etis */
