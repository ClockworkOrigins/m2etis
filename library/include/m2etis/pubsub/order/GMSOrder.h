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
 * \addtogroup pubsub
 * @{
 */

#ifndef __M2ETIS_PUBSUB_ORDER_GMSORDER_H__
#define __M2ETIS_PUBSUB_ORDER_GMSORDER_H__

#include <set>

#include "m2etis/message/info/order/GMSOrderInfo.h"
#include "m2etis/pubsub/order/BaseOrder.h"

#include "boost/any.hpp"

namespace m2etis {
namespace pubsub {
namespace order {

	/**
	 * \brief implements the Order Strategy from Garcia-Molina and Spauster
	 * The Strategy ensures synchronisation along multiple trees.
	 * Doesn't work with RoutingStrategies sending directly Notify messages instead of Published to root, e.g. DirectBroadcastRouting
	 */
	template<class NetworkType, unsigned int Timeout>
	class GMSOrder : public BaseOrder<NetworkType> {
		/**
		* \brief struct representing an message
		*/
		struct MsgInfo {
			MsgInfo(uint64_t a, uint64_t b, uint64_t c) : seqNr(a), treeId(b), msgId(c) {
			}

			/**
			 * \brief sequence number
			 */
			uint64_t seqNr;

			/**
			 * \brief tree on which this message shall be delivered
			 */
			uint64_t treeId;

			/**
			 * \brief msgNr used to reference the message in the tree buffer
			 */
			uint64_t msgId;

			bool operator<(const MsgInfo & b) const {
				return !(std::tie(seqNr, treeId, msgId) < std::tie(b.seqNr, b.treeId, b.msgId));
			}

			bool operator>(const MsgInfo & b) const {
				return !(*this < b);
			}
		};

	public:
		typedef message::GMSOrderInfo<NetworkType> OrderInfoType;

		GMSOrder(PubSubSystemEnvironment * pssi, bool isRoot) : BaseOrder<NetworkType>(pssi, isRoot), pssi_(pssi), nextFinished(0), nextSend(0) {
			// if rootnode, set variables that wouldn't be set because we don't get a subscribe msg
			if (isRoot) {
				for (size_t i = 0; i < others_.size(); ++i) {
					others_[i]->setMasterTree(myTree_);
				}
			}
		}

		virtual ~GMSOrder() {}

		uint64_t counter = 0;

		/**
		 * \brief pointer to the PubSubSystemEnvironment
		 */
		PubSubSystemEnvironment * pssi_;

		/**
		 * \brief list of all Order classes from the other trees
		 */
		std::vector<GMSOrder *> others_;

		/**
		 * \brief for every tree, save all subscribers
		 * only neccessary on root node
		 */
		std::vector<std::set<typename NetworkType::Key>> subs_;

		/**
		 * \brief Receiver -> next Seq. Nr. to use
		 */
		std::map<typename NetworkType::Key, uint64_t> nextSend_;
		/**
		 * \brief Sender -> next Seq. Nr. to expect
		 */
		std::map<typename NetworkType::Key, uint64_t> nextRec_;
		/**
		 * \brief Receiver -> Tree that should be used for the Control msg
		 * only used in root node
		 */
		std::map<typename NetworkType::Key, uint64_t> mainTree_;

		/**
		 * \brief this node is subscribed on these trees
		 */
		std::set<uint64_t> selfSub_;

		/**
		 * \brief next ID to be expected
		 */
		uint64_t nextFinished;

		/**
		 * \brief tree on which Control Msgs are expected. Default: None
		 */
		uint64_t masterTree = UINT64_MAX;

		/**
		 * \brief index number of this tree
		 */
		uint64_t myTree_ = UINT64_MAX;

		/**
		 * \brief next number to be used as the global Sequence number
		 */
		uint64_t nextSend;

		/**
		 * \brief which node is subscribed on which tree
		 */
		std::map<typename NetworkType::Key, std::set<uint64_t>> subOn_;

		typedef std::pair<uint64_t, uint64_t> MsgIdent; // <Tree, MsgNr>
		typedef uint64_t SN;
		typedef std::pair<uint64_t, uint64_t> BufId; // <Tree, BufferNr>

		typedef std::tuple<MsgInfo, uint64_t> QueueTuple;

		/**
		 * \brief Queue containing all recieved but not yet further processed ctrl messages
		 */
		std::priority_queue<QueueTuple, std::vector<QueueTuple>, std::greater<QueueTuple>> ctrlQueue_;

		/**
		 * \brief stores all NOTIFY msgs that arrived
		 */
		std::map<MsgIdent, BufId> pubQueue_;

		/**
		 * \brief returns whether pending messages exist
		 */
		bool hasPending() {
			return !ctrlQueue_.empty();
		}

		/**
		 * \brief this node will only accept controlmessages from this tree
		 */
		void setMasterTree(uint64_t t) {
			masterTree = t;
		}

		/**
		 * \brief configures the info struct
		 */
		bool configureOrderInfo(uint64_t id, const message::ActionType mtype, typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & rec) {
			// no infos, just send
			typename OrderInfoType::Ptr info = boost::static_pointer_cast<OrderInfoType>(ptr);
			info->mT = masterTree;
			info->sender = BaseOrder<NetworkType>::hn;

			if (mtype == message::SUBSCRIBE) {
				info->realTree = myTree_;
				if (masterTree == UINT64_MAX) { // not yet subscribed on any tree
					info->mT = myTree_;
					for (size_t i = 0; i < others_.size(); ++i) {
						others_[i]->setMasterTree(myTree_);
					}
					BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				} else if (masterTree == myTree_) {
					BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				} else {
					BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				}
				// remember this subscription
				if (selfSub_.find(myTree_) == selfSub_.end()) {
					selfSub_.insert(myTree_);
				}
			} else if (mtype == message::PUBLISH) {
				info->realTree = myTree_;
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
			} else if (mtype == message::NOTIFY) {
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
			} else if (mtype == message::CONTROL) {
				if (info->seqNr == 123456789 && info->type != message::TYPE::UNSUB) { // identify my own control msgs
					if (nextSend_.find(rec) == nextSend_.end()) { // send with next id for this link
						nextSend_[rec] = 0;
					}
					info->seqNr = nextSend_[rec]++;
				}
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
			} else {
				selfSub_.erase(myTree_);
				if (myTree_ == masterTree) {
					// we need a new masterTree
					if (selfSub_.empty()) { // no more subscriptions
						masterTree = UINT64_MAX;
					} else {
						masterTree = *(selfSub_.begin());
					}
				}
				// send an unsubscribe control to server
				typename OrderInfoType::Ptr p = boost::make_shared<OrderInfoType>();
				p->type = message::TYPE::UNSUB;
				p->realTree = myTree_;
				p->mT = masterTree;
				p->seqNr = 123456789; // random value to identify these messages in configure order info
				BaseOrder<NetworkType>::sendMsg_(p, typename NetworkType::Key(), ControlTarget::ROOT);

				info->realTree = myTree_;
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
			}
			return true;
		}

		/**
		 * \brief processes a subscribe payload (Root)
		 * the message will be processed on tree 0
		 */
		void processSubscribePayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			typename OrderInfoType::Ptr info = boost::static_pointer_cast<OrderInfoType>(ptr);
			if (BaseOrder<NetworkType>::isRoot_) {
				if (myTree_ == 0) {
					// process on tree 0
					info->realTree = info->realTree == UINT64_MAX ? 0 : info->realTree;
					// sender is subscribed on tree realTree
					subOn_[sender].insert(info->realTree);
					// on realTree, sender is subscribed
					subs_[uint32_t(info->realTree)].insert(sender);
					// sender's maintree is mT
					mainTree_[sender] = info->mT;
				} else {
					//redirect msg to tree 0
					info->realTree = myTree_;
					others_[0]->processSubscribePayload(ptr, sender);
				}
			}
		}

		void processUnsubscribePayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) {
		}

		/**
		 * \brief processes a publish payload
		 */
		void processPublishPayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key &) {
			typename OrderInfoType::Ptr info = boost::static_pointer_cast<OrderInfoType>(ptr);
			assert(BaseOrder<NetworkType>::isRoot_); // only the root node receives publish messages
			info->msgNr = nextSend++; // give it a unique id
			// Tree 0 shall send the CtrlMsgs used for this message (This tree sends the Msg on all needed trees)
			others_[0]->sendCtrl(myTree_, info->msgNr);
		}

		/**
		 * \brief called for every NotifyMsg that arrived
		 */
		void processNotifyPayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief processes a control payload (Root)
		 */
		bool processControlPayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			typename OrderInfoType::Ptr info = boost::static_pointer_cast<OrderInfoType>(ptr);
			if (info->type == message::TYPE::UNSUB) {
				// someone unsubscribes
				if (BaseOrder<NetworkType>::isRoot_) {
					if (myTree_ == 0) {
						// process on tree 0
						info->realTree = info->realTree == UINT64_MAX ? 0 : info->realTree;
						// sender is subscribed on tree realTree
						subOn_[sender].erase(info->realTree);
						// on realTree, sender is subscribed
						subs_[uint32_t(info->realTree)].erase(sender);
						// sender's maintree is mT
						mainTree_[sender] = info->mT;
					} else {
						//redirect msg to tree 0
						info->realTree = myTree_;
						others_[0]->processControlPayload(ptr, sender);
					}
				}
				return false;
			}

			if (masterTree != myTree_) {
				// ControlMsg on wrong Tree. Just ignore the message
				return true;
			}
			ctrlQueue_.push(std::make_tuple(MsgInfo(info->seqNr, info->realTree, info->msgNr), BaseOrder<NetworkType>::pssi_->scheduler_.getTime()));
			updateQueue();
			return true;
		}

		/**
		 * \brief the Message with id a shall be delivered
		 * This is used to force other instances to deliver a message
		 */
		void deliver(uint64_t a) {
			BaseOrder<NetworkType>::function_(a, msgProcess::MSG_PROCESS_AND_DELETE);
		}

		/**
		 * \brief checks the queue for new messages to deliver
		 */
		void updateQueue() {
			bool b = true;
			while (b && !ctrlQueue_.empty()) { // process multiple messages
				b = false; // stop unless we remove the first element
				MsgInfo head = std::get<0>(ctrlQueue_.top());
				if (ctrlQueue_.size() > 0 && head.seqNr == nextFinished) { // next message to be processed
					if (head.msgId == UINT64_MAX) { // this message doesn't need to be synchronised
						nextFinished++;
						ctrlQueue_.pop();
						b = true;
					} else {
						BufId bid = std::make_pair(head.treeId, head.msgId); // identifier for a message in the buffer (tree + id)
						// check if corresponding NOTIFY already arrived
						if (pubQueue_.find(bid) != pubQueue_.end()) {
							// deliver the message with given id on it's realTree
							others_[uint32_t(pubQueue_[bid].first)]->deliver(pubQueue_[bid].second);
							pubQueue_.erase(bid);
							ctrlQueue_.pop();
							nextFinished++;
							b = true;
						}
					}
				} else if (std::get<1>(ctrlQueue_.top()) + Timeout > BaseOrder<NetworkType>::pssi_->scheduler_.getTime()) {
					// Timeout
					nextFinished = std::get<0>(ctrlQueue_.top()).seqNr;
					b = true;
				}
			}
		}

		/**
		 * \brief called by the Tree whenever a message is ready to be delivered
		 * This function stores the messages (to be precise, the corresponding ids)
		 * until the controlmessages with orderinformation arrived
		 */
		void receive(uint64_t id, typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			typename OrderInfoType::Ptr info = boost::static_pointer_cast<OrderInfoType>(ptr);
			if (BaseOrder<NetworkType>::isRoot_) {
				// no need to buffer messages on the root because they are in order by definition
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				return;
			}
			if (masterTree == UINT64_MAX || myTree_ == masterTree) {
				// save the message until the corresponding controlmessage arrived
				pubQueue_[std::make_pair(info->realTree, info->msgNr)] = std::make_pair(info->realTree, id);
				updateQueue();
			} else {
				// redirect message to the master Tree
				others_[uint32_t(masterTree)]->receive(id, ptr, sender);
			}
		}

		/**
		 * \brief stores the instances of the OrderStrategy from the other Channels
		 * Also sets the myTree_ variable and initializes subs_
		 * \param[in] others list of all order instances from this channel
		 */
		void otherOrders(const std::vector<GMSOrder *> & others) {
			others_ = others; // save other instances

			// searches instances to find the index of the current tree
			for (size_t i = 0; i < others_.size(); ++i) {
				if (others_[i] == this) {
					myTree_ = i;
					break;
				}
			}

			// initializes lists of subscribers for all trees
			subs_ = std::vector<std::set<typename NetworkType::Key>>(others_.size());
		}

		/**
		 * \brief the tree shall publish CtrlMsgs on all trees that have subscribers
		 * This function is only used on Tree 0
		 * \param[in] treeId Id of the Tree where the actual message is published
		 * \param[in] msgId Sequence number used for this message
		 */
		void sendCtrl(uint64_t treeId, uint64_t msgId) {
			std::set<uint64_t> v;
			for (typename std::set<typename NetworkType::Key>::iterator subsOnTree = subs_[uint32_t(treeId)].begin(); subsOnTree != subs_[uint32_t(treeId)].end(); ++subsOnTree) { // *subsOnTree := string
				// iterator over all subscribers on the tree and get all used mainTrees
				v.insert(mainTree_[*subsOnTree]);
			}
			// send a CtrlMsg on all these maintrees
			for (std::set<uint64_t>::iterator it = v.begin(); it != v.end(); ++it) {
				others_[uint32_t(*it)]->sendCtrl2(treeId, msgId);
			}
		}

		/**
		 * \brief send a ctrlMsg on this tree
		 * \param[in] treeId the real Tree where the message will be sent
		 * \param[in] msgId The Id used for the message
		 */
		void sendCtrl2(uint64_t treeId, uint64_t msgId) {
			typename OrderInfoType::Ptr p = boost::make_shared<OrderInfoType>();
			p->msgNr = msgId;
			p->realTree = treeId;
			// TODO: (Daniel) remove magic number!
			p->seqNr = 123456789; // random value to identify these messages in configure order info
			// publish this message normally
			BaseOrder<NetworkType>::sendMsg_(p, typename NetworkType::Key(), ControlTarget::ALL);
		}

		/**
		 * \brief notified when message dropped by filter or validity strategy
		 */
		void notifyRemovedMessage(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}
	};

} /* namespace order */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ORDER_GMSORDER_H__ */

/**
 * @}
 */
