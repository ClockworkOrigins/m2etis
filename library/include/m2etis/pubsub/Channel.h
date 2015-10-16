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

/**
 * @author Johannes Held
 * @detail Handlerclass for one Channel
 */

/**
 * \addtogroup pubsub
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_CHANNEL_H__
#define __M2ETIS_PUBSUB_CHANNEL_H__

#include "m2etis/util/Logger.h"

#include "m2etis/TemplateHelper.h"
#include "m2etis/message/MessageFactory.h"
#include "m2etis/message/MessageType.h"
#include "m2etis/message/info/OrderInfo.h"
#include "m2etis/net/NetworkController.h"
#include "m2etis/net/NetworkFactory.h"
#include "m2etis/parameters/QueueParameters.h"
#include "m2etis/pubsub/ChannelEventInterface.h"
#include "m2etis/pubsub/ChannelInterface.h"
#include "m2etis/pubsub/DeliverCallbackInterface.h"
#include "m2etis/pubsub/DeliverInfo.h"
#include "m2etis/pubsub/PubSubSystemEnvironment.h"
#include "m2etis/pubsub/TreeFactory.h"
#include "m2etis/pubsub/config/ChannelType.h"

#include "m2etis/sim/i6eRandom.h"

#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/foreach.hpp"
#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread/locks.hpp"

#if I6E_PLATFORM == I6E_PLATFORM_WIN32
	#pragma warning(push)
	#pragma warning(disable : 4127)
#endif

namespace m2etis {
namespace pubsub {

	/**
	 * \ingroup pubsub
	 *
	 * \class Channel
	 * \brief Channel
	 *
	 * Long Desc
	 *
	 */
	template<class ChannelType, class NetworkType, class EventType>
	class Channel : public BasicChannelInterface<EventType>, public ChannelEventInterface, public ChannelType::PartitionStrategy, public ChannelType::RendezvousStrategy {
		typedef message::InternalMessage<NetworkType, ChannelType, EventType> IMessage;
		typedef Tree<ChannelType, NetworkType, EventType> TreeType;

		typename NetworkType::Key _self;
		typename NetworkType::Key _rendezvous;
		message::MessageFactory<ChannelType, NetworkType> factory_;
		const ChannelName topic_;
		net::NetworkController<NetworkType> * controller_;
		std::vector<TreeType *> trees_;
		util::DoubleBufferQueue<typename message::M2Message<EventType>::Ptr> msgQueue_;
		util::DoubleBufferQueue<std::pair<BasicDeliverCallbackInterface<EventType> *, boost::shared_ptr<filter::FilterExp<EventType>>>> subscribeQueue_;
		util::DoubleBufferQueue<boost::shared_ptr<filter::FilterExp<EventType>>> unsubscribeQueue_; // for filter strategies
		bool unsubscribe_;
		PubSubSystemEnvironment * pssi_;
		std::map<typename NetworkType::Key, uint64_t> _nodeList;
		bool joined;
		bool _hasSubscribe;
		std::pair<BasicDeliverCallbackInterface<EventType> *, boost::shared_ptr<filter::FilterExp<EventType>>> _lastSubscribe;
		uint64_t parseID_;
		uint64_t dynamicPeriodID_;

	public:
		/**
		 * \brief Constructor
		 */
		Channel(const ChannelName topic_name, const std::string & ip, uint16_t port, const std::string & rendezvousIP, uint16_t known_hostport, PubSubSystemEnvironment * pssi, const std::vector<std::string> & rootList) : ChannelType::PartitionStrategy(), ChannelType::RendezvousStrategy(rootList), _self(ip + std::string(":") + std::to_string(port)), _rendezvous(rendezvousIP + std::string(":") + std::to_string(known_hostport)), factory_(message::MessageFactory<ChannelType, NetworkType>()), topic_(topic_name), controller_(pssi->_factory->createNetworkController(NetworkType())), msgQueue_(), subscribeQueue_(), unsubscribeQueue_(), unsubscribe_(false), pssi_(pssi), _nodeList({std::make_pair(_self, 0)}), joined(false), _hasSubscribe(false), _lastSubscribe(), dynamicPeriodID_(UINT64_MAX) {
			if (!ChannelType::PartitionStrategy::DYNAMIC_PARTITION) {
				for (auto name : ChannelType::PartitionStrategy::getTreeNames()) {
					trees_.push_back(pssi->_tree_factory->template createTree<ChannelType, NetworkType, EventType>(_self, _rendezvous, typename NetworkType::Key(ChannelType::RendezvousStrategy::getRoot() + ":" + std::to_string(known_hostport)), pssi, topic_));
					if (name) {} // Just to prevent unused warning
				}
			} else {
				ChannelType::PartitionStrategy::createRendezvousPartition(_rendezvous);

				for (auto name : ChannelType::PartitionStrategy::getTreeNames()) {
					trees_.push_back(pssi->_tree_factory->template createTree<ChannelType, NetworkType, EventType>(_self, _rendezvous, _rendezvous, pssi, topic_));
					if (name) {} // Just to prevent unused warning
				}
			}

			updateOrderTrees();

			parseID_ = pssi->scheduler_.runRepeated(parameters::PULL_PARSEMESSAGE, boost::bind(&Channel::parseMessages, this), 2);
			controller_->register_deliver(message::JOIN | topic_, boost::bind(&Channel::deliver, this, _1));
			controller_->register_deliver(message::STATE | topic_, boost::bind(&Channel::deliver, this, _1));
			controller_->register_deliver(message::LEAVE | topic_, boost::bind(&Channel::deliver, this, _1));

			if (_self != _rendezvous) {
				pssi->scheduler_.runOnce(1000, boost::bind(&Channel::startJoin, this), 1);
			} else {
				dynamicPeriodID_ = pssi->scheduler_.runRepeated(6000000, boost::bind(&Channel::purgeNodes, this), 1);
				joined = true;
			}
		}

		virtual ~Channel() {
			pssi_->scheduler_.stop(parseID_);
			if (dynamicPeriodID_ != UINT64_MAX) {
				pssi_->scheduler_.stop(dynamicPeriodID_);
			}
			msgQueue_.clear();
			for (TreeType * t : trees_) {
				delete t;
			}
			trees_.clear();

			if (_self != _rendezvous) {
#ifdef WITH_SIM
				SimulationEventType v;
				v._simID = -1;
				v._simChannel = topic_;
				typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(createMessage(v));
#else
				typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(createMessage());
#endif /* WITH_SIM */

				msg->sender = _self;
				msg->receiver = _rendezvous;
				msg->type = message::LEAVE | topic_;
				controller_->send(msg);
			}
		}

		inline std::string getSelf() const {
			// all trees belonging to the same topic have a reference to the same NetworkController
			// arbitrariliy choosing the first tree:
			return trees_[0]->getSelf().toStr();
		}

		inline ChannelName getChannel() const {
			return topic_;
		}

		void subscribe(BasicDeliverCallbackInterface<EventType> & callback, boost::shared_ptr<filter::FilterExp<EventType> > predicate) {
			subscribeQueue_.push(std::make_pair(&callback, predicate));
		}

		// for filter strategies: deregistering a single filter: (unsubscribes from the tree, if last filter has been deregistered)
		void unsubscribe(boost::shared_ptr<filter::FilterExp<EventType>> predicate) override {
			unsubscribeQueue_.push(predicate);
		}

		void unsubscribe() {
			unsubscribe_ = true;
		}

		void publish(const typename message::M2Message<EventType>::Ptr msg) {
			msgQueue_.push(msg);
		}

		typename message::M2Message<EventType>::Ptr createMessage() const {
			return factory_.template createMessage<EventType>();
		}

		typename message::M2Message<EventType>::Ptr createMessage(const EventType & payload) const {
			return factory_.createMessage(payload);
		}

#ifdef WITH_SIM
		typename message::NetworkMessage<net::NetworkType<net::OMNET> >::Ptr transformToNetworkMessage(typename message::M2Message<EventType>::Ptr msg) const {
			return factory_.template transformToNetworkMessage<EventType>(msg);
		}

		typename message::M2Message<EventType>::Ptr transformToM2Message(typename message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr msg) const {
			return factory_.template transformToM2Message<EventType>(msg);
		}
#endif

		bool parseMessages() {
			while (!subscribeQueue_.empty()) {
				_lastSubscribe = subscribeQueue_.poll();
				_hasSubscribe = true;

				for (auto tree_index : ChannelType::PartitionStrategy::getSubscribeTrees(_lastSubscribe.second)) {
					trees_[tree_index]->subscribe(*_lastSubscribe.first, _lastSubscribe.second);
				}
			}

			if (unsubscribe_) {
				for (auto tree : trees_) {
					tree->unsubscribe();
				}

				unsubscribe_ = false;
			}

			// for filter strategies: process unsubscribeQueue
			while (!unsubscribeQueue_.empty()) {
				boost::shared_ptr<filter::FilterExp<EventType> > deregisteredFilter = unsubscribeQueue_.poll();

				for (auto tree_index : ChannelType::PartitionStrategy::getSubscribeTrees(deregisteredFilter)) {
					trees_[tree_index]->unsubscribe(deregisteredFilter);
				}
			}

			uint32_t msgPublished = 0; // pause publishing after some messages to allow other task running
			while (msgPublished < parameters::PUBLISH_MESSAGECOUNT_MAX && !msgQueue_.empty()) {
				typename message::M2Message<EventType>::Ptr msg = msgQueue_.poll();
				trees_[ChannelType::PartitionStrategy::getPublishTree(msg->payload, _self)]->publish(msg);
				++msgPublished;
			}
			return true;
		}

		/**
		 * \brief Channel receives messages from network in here, used for dynamic partitions
		 */
		void deliver(typename message::NetworkMessage<NetworkType>::Ptr msg) {
			typename IMessage::Ptr msg2 = boost::static_pointer_cast<IMessage>(msg);

			message::ActionType tmp = message::ActionType(msg2->type & message::ACTION_TYPE_MASK);

			if (tmp == message::JOIN) {
				if (_self == _rendezvous) {
					if (_nodeList.find(msg2->sender) == _nodeList.end()) { // current state hasn't to be sent if only a node updated its state
						_nodeList[msg2->sender] = pssi_->clock_.getTime(); // add node to list and set time for this node to be able to purge it

						if (ChannelType::PartitionStrategy::createPartition(msg2->sender)) {
							trees_.push_back(pssi_->_tree_factory->template createTree<ChannelType, NetworkType, EventType>(_self, _rendezvous, msg2->sender, pssi_, topic_));

							updateOrderTrees();

							if (_hasSubscribe) {
								unsubscribePartitions();
							}
						}

						// RP answers with current node list AND its own time
						for (std::pair<typename NetworkType::Key, uint64_t> node : _nodeList) {
							msg2->_nodeList.push_back(node.first);
						}

						msg2->sender = _self;
						msg2->type = message::STATE | topic_;
						msg2->_time = pssi_->clock_.getTime();

						if (ChannelType::PartitionStrategy::DYNAMIC_PARTITION) {
							for (size_t i = 0; i < trees_.size(); ++i) {
								msg2->_trees.push_back(typename IMessage::TreeHelper(trees_[i]->getTopic(), ChannelType::PartitionStrategy::getPredicate(i), trees_[i]->getRoot()));
							}
						}

						for (typename NetworkType::Key node : msg2->_nodeList) {
							if (node != _self) { // RP mustn't send message to itself
								msg2->receiver = node;
								controller_->send(boost::make_shared<IMessage>(*msg2));
							}
						}

						if (_hasSubscribe) {
							subscribe(*_lastSubscribe.first, _lastSubscribe.second);
						}
					} else {
						_nodeList[msg2->sender] = pssi_->clock_.getTime();
					}
				}
			} else if (tmp == message::STATE) {
				joined = true;
				for (typename NetworkType::Key node : msg2->_nodeList) {
					_nodeList[node] = 0;
				}

				pssi_->clock_.setOffset(int64_t(msg2->_time) - int64_t(pssi_->clock_.getRealTime()) + int64_t(parameters::EXPECTED_LATENCY)); // difference between the two clocks + network delay

				if (ChannelType::PartitionStrategy::DYNAMIC_PARTITION) {
					bool newTree = false;
					for (size_t i = 0; i < msg2->_trees.size(); ++i) {
						bool found = false;
						for (size_t j = 0; j < trees_.size(); ++j) {
							if (trees_[j]->getTopic() == msg2->_trees[i].topic) { // if tree with this topic already exists skip it
								ChannelType::PartitionStrategy::changePredicate(j, msg2->_trees[i].predicates);
								ChannelType::PartitionStrategy::changeRoot(j, msg2->_trees[i].root);
								found = true;
								break;
							}
						}
						if (!found) {
							ChannelType::PartitionStrategy::addPartition(msg2->_trees[i].predicates, msg2->_trees[i].root); // insert partition to partition strategy
							trees_.push_back(new Tree<ChannelType, NetworkType, EventType>(msg2->_trees[i].topic, _self, _rendezvous, msg2->_trees[i].root, pssi_, topic_)); // create responding tree

							newTree = true;
						}
					}

					if (newTree && _hasSubscribe) {
						updateOrderTrees();

						unsubscribePartitions();

						subscribe(*_lastSubscribe.first, _lastSubscribe.second);
					}
				}
			} else if (tmp == message::LEAVE) {
				_nodeList.erase(msg2->sender);

				if (_self == _rendezvous) {
					if (ChannelType::PartitionStrategy::DYNAMIC_PARTITION) {
						for (size_t i = 0; i < trees_.size(); ++i) {
							if (trees_[i]->getRoot() == msg2->sender) {
								msg2->_topics.insert(trees_[i]->getTopic());
							}
						}

						removeTopics(msg2->_topics);

						updateOrderTrees();

						if (_hasSubscribe) {
							unsubscribePartitions();

							subscribe(*_lastSubscribe.first, _lastSubscribe.second);
						}
					}

					for (std::pair<typename NetworkType::Key, uint64_t> node : _nodeList) {
						if (node.first != _self) { // RP mustn't send message to itself
							msg2->receiver = node.first;
							controller_->send(boost::make_shared<IMessage>(*msg2));
						}
					}
				} else {
					if (ChannelType::PartitionStrategy::DYNAMIC_PARTITION) {
						removeTopics(msg2->_topics);

						updateOrderTrees();

						if (_hasSubscribe) {
							unsubscribePartitions();

							subscribe(*_lastSubscribe.first, _lastSubscribe.second);
						}
					}
				}
			}
		}

	private:
		/**
		 * \brief called by every node except RP, sends message to RP to inform it about the new node
		 */
		bool startJoin() {
			updateState();

			dynamicPeriodID_ = pssi_->scheduler_.runRepeated(5000000, boost::bind(&Channel::updateState, this), 1);

			return false;
		}

		/**
		 * \brief purges nodes not updating their state anymore
		 */
		bool purgeNodes() {
			assert(_self == _rendezvous);
			std::vector<typename NetworkType::Key> purges;

			for (std::pair<typename NetworkType::Key, uint64_t> p : _nodeList) {
				if (p.first != _rendezvous && pssi_->clock_.getTime() - p.second >= 6000000) {
					purges.push_back(p.first);
				}
			}

			for (typename NetworkType::Key node : purges) {
				_nodeList.erase(node);
			}

			for (typename NetworkType::Key node : purges) {
#ifdef WITH_SIM
				SimulationEventType v;
				v._simID = -1;
				v._simChannel = topic_;
				typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(createMessage(v));
#else
				typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(createMessage());
#endif /* WITH_SIM */

				msg->sender = node;
				msg->type = message::LEAVE | topic_;

				if (ChannelType::PartitionStrategy::DYNAMIC_PARTITION) {
					for (size_t i = 0; i < trees_.size(); ++i) {
						if (trees_[i]->getRoot() == node) {
							msg->_topics.insert(trees_[i]->getTopic());
						}
					}

					removeTopics(msg->_topics);

					updateOrderTrees();

					if (_hasSubscribe) {
						unsubscribePartitions();

						subscribe(*_lastSubscribe.first, _lastSubscribe.second);
					}
				}

				for (std::pair<typename NetworkType::Key, uint64_t> nodes : _nodeList) {
					if (nodes.first != _self) { // RP mustn't send message to itself
						msg->receiver = nodes.first;
						controller_->send(boost::make_shared<IMessage>(*msg));
					}
				}
			}

			return true;
		}

		/**
		 * \brief sends new join message to inform RP that this node is still alive
		 */
		bool updateState() {
#ifdef WITH_SIM
			SimulationEventType v;
			v._simID = -1;
			v._simChannel = topic_;
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(createMessage(v));
#else
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(createMessage());
#endif /* WITH_SIM */

			msg->sender = _self;
			msg->receiver = _rendezvous;
			msg->type = message::JOIN | topic_;
			controller_->send(msg);

			return true;
		}

		/**
		 * \brief updates trees for order strategies
		 */
		void updateOrderTrees() {
			std::vector<typename ChannelType::OrderStrategy *> v;
			for (size_t i = 0; i < trees_.size(); ++i) {
				v.push_back(static_cast<typename ChannelType::OrderStrategy *>(trees_[i]));
			}
			for (size_t i = 0; i < trees_.size(); i++) {
				trees_[i]->otherOrders(v);
			}
		}

		/**
		 * \brief removes list of given topics
		 */
		void removeTopics(const std::set<unsigned int> & topics) {
			for (unsigned int ttr : topics) {
				for (size_t j = 0; j < trees_.size(); ++j) {
					if (trees_[j]->getTopic() == ttr) {
						ChannelType::PartitionStrategy::removePartition(j);
						delete trees_[j];
						trees_.erase(trees_.begin() + int(j));
						break;
					}
				}
			}
		}

		/**
		 * \brief unsubscribes from trees not matching after partition change
		 */
		void unsubscribePartitions() {
			std::vector<unsigned int> subs = ChannelType::PartitionStrategy::getSubscribeTrees(_lastSubscribe.second);

			size_t index = 0;

			for (size_t i = 0; i < trees_.size(); ++i) {
				if (i == subs[index]) {
					index++;
				} else {
					trees_[i]->unsubscribe(_lastSubscribe.second);
				}
			}
		}
	};

} /* namespace pubsub */
} /* namespace m2etis */

#if I6E_PLATFORM == I6E_PLATFORM_WIN32
	#pragma warning(pop)
#endif

#endif /* __M2ETIS_PUBSUB_CHANNEL_H__ */

/*
 * @}
 */
