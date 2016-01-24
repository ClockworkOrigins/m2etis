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

#ifndef __M2ETIS_PUBSUB_ORDER_MTPORDER_H__
#define __M2ETIS_PUBSUB_ORDER_MTPORDER_H__

#include <queue>
#include <tuple>

#include "m2etis/message/info/order/MTPOrderInfo.h"
#include "m2etis/pubsub/order/BaseOrder.h"
#include "m2etis/pubsub/order/RemoveHelper.h"

#include "boost/thread.hpp"

#if I6E_PLATFORM == I6E_PLATFORM_WIN32
	#pragma warning(push)
	#pragma warning(disable : 4127)
#endif

namespace m2etis {
namespace pubsub {
namespace order {

	/**
	 * \brief what to do when recieving an old message after Timeout
	 */
	enum class LateDeliver {
		DELIVER,
		DROP
	};

	/**
	 * \brief This class implements the MTP Order strategy
	 * The root node is used as a fixed sequencer. Every node that wants to send a message,
	 * request a sequenzer number (TOKEN) from the sequencer
	 * and afterwards sends his message along with this sequence number
	 */
	template<class NetworkType, uint64_t Timeout, LateDeliver deli>
	class MTPOrder : public BaseOrder<NetworkType> {
	public:
		typedef message::MTPOrderInfo OrderInfoType;

		MTPOrder(PubSubSystemEnvironment * pssi, bool b) : BaseOrder<NetworkType>(pssi, b), queue_(), nextRec_(0), nextSend_(0), delivered_(0), periodicID_(0) {
			periodicID_ = pssi->scheduler_.runRepeated(50000, boost::bind(&MTPOrder::purge, this), 1);
		}

		virtual ~MTPOrder() {
			BaseOrder<NetworkType>::pssi_->scheduler_.stop(periodicID_);
		}

		/**
		 * \brief pending messages if the buffer is non-empty
		 */
		bool hasPending() {
			return !queue_.empty();
		}

		/**
		 * \brief used to manage sending a message
		 * All messages but Publish messages can be sent directly without any special treatment
		 * Publish messages however need to be stored until a sequence number is available
		 */
		bool configureOrderInfo(uint64_t id, const message::ActionType mtype, typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & rec) {
			message::MTPOrderInfo::Ptr info = boost::static_pointer_cast<message::MTPOrderInfo>(ptr);
			info->missing_ = remHelp.getDropped(rec);
			remHelp.clear(rec);
			
			if (mtype == message::NOTIFY) { // notify already as sequence number
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				return true;
			}
			if (mtype == message::SUBSCRIBE) { // subscribes don't need a sequence number
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				return true;
			}
			if (mtype == message::UNSUBSCRIBE) { // unsubscribes don't need a sequence number
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				return true;
			}
			if (mtype == message::CONTROL) { // controls don't need a sequence number
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				return true;
			}
			sendQueue_.push(std::make_pair(id, info)); // store the current message buffer id along with the header struct
			OrderInfoType::Ptr p = boost::make_shared<OrderInfoType>();
			p->type_ = message::MTPOrderInfo::TYPE_TOKEN_REQUEST;
			BaseOrder<NetworkType>::sendMsg_(p, typename NetworkType::Key(), ControlTarget::ROOT); // send a new control message to the root node of this tree (sequencer) to get a sequence number
			return true;
		}

		/**
		 * \brief processes a control message
		 */
		bool processControlPayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			message::MTPOrderInfo::Ptr info = boost::static_pointer_cast<message::MTPOrderInfo>(ptr);
			if (info->type_ == message::MTPOrderInfo::TYPE_TOKEN_REQUEST) {
				// (Root) an id was requested
				OrderInfoType::Ptr p = boost::make_shared<OrderInfoType>();
				p->type_ = message::MTPOrderInfo::TYPE_TOKEN_GRANT;
				p->seqNr = nextSend_++;
				BaseOrder<NetworkType>::sendMsg_(p, sender, ControlTarget::SINGLE);
			} else {
				// (Sender) received an id. sending...
				assert(info->type_ == message::MTPOrderInfo::TYPE_TOKEN_GRANT);
				assert(sendQueue_.size() > 0);
				sendQueue_.front().second->seqNr = info->seqNr;
				BaseOrder<NetworkType>::function_(sendQueue_.front().first, msgProcess::MSG_PROCESS_AND_DELETE);
				sendQueue_.pop();
			}
			return false;
		}

		/**
		 * \brief processes a subscribe message
		 */
		void processSubscribePayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief processes a subscribe message
		 */
		void processPublishPayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief called for every NotifyMsg that arrived
		 */
		void processNotifyPayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key &) {
			message::MTPOrderInfo::Ptr info = boost::static_pointer_cast<message::MTPOrderInfo>(ptr);

			for (uint64_t i : info->missing_) {
				droppedQueue_.push(i);
			}

			purge();
		}

		/**
		 * \brief processes a subscribe message
		 */
		void otherOrders(const std::vector<MTPOrder *> &) {
		}

		/**
		 * \brief receives a message
		 * stores the message in the queue and wait for it's deliver
		 */
		void receive(uint64_t id, typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key &) {
			message::MTPOrderInfo::Ptr info = boost::static_pointer_cast<message::MTPOrderInfo>(ptr);
			
			if (info->seqNr == nextRec_) {
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				delivered_++;
				nextRec_++;
				purge();
			} else if (info->seqNr == UINT64_MAX) {
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				delivered_++;
				nextRec_++;
				purge();
			} else if (info->seqNr < nextRec_) {
				// message arrived too late
				if (deli == LateDeliver::DELIVER) {
					// deliver
					BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				} else {
					// just delete from buffer
					BaseOrder<NetworkType>::function_(id, msgProcess::MSG_DELETE);
				}
			} else {
				queue_.push(std::make_tuple(info->seqNr, id, BaseOrder<NetworkType>::pssi_->scheduler_.getTime()));
			}
		}

		/**
		 * \brief notified when message dropped by filter or validity strategy
		 */
		void notifyRemovedMessage(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & receiver) {
			message::MTPOrderInfo::Ptr info = boost::static_pointer_cast<message::MTPOrderInfo>(ptr);
			remHelp.dropped(info->seqNr, receiver);
		}

	private:
		/**
		 * \brief stores arrived messages
		 * <SequenceNumber, Number in Buffer, Time of arrival>
		 */
		std::priority_queue<std::tuple<uint64_t, uint64_t, uint64_t>, std::vector<std::tuple<uint64_t, uint64_t, uint64_t>>, std::greater<std::tuple<uint64_t, uint64_t, uint64_t>>> queue_;
		
		/**
		 * \brief stores messages to be sent until a SeqNr was 
		 * <buffer id, msgPointer>
		 */
		std::queue<std::pair<uint64_t, OrderInfoType::Ptr>> sendQueue_;
		
		/**
		 * \brief stores message ids that were dropped by parents
		 */
		std::priority_queue<uint64_t, std::vector<uint64_t>, std::greater<uint64_t>> droppedQueue_;
		
		/**
		 * \brief next id to be delivered
		 */
		uint64_t nextRec_;
		
		/**
		 * \brief id for the next message (on root only)
		 */
		uint64_t nextSend_;
		
		/**
		 * \brief number of messages delivered (just for interest)
		 */
		uint64_t delivered_;
		
		/**
		 * \brief stores data about droppen messages
		 */
		RemoveHelper<uint64_t, typename NetworkType::Key> remHelp;

		uint64_t periodicID_;

		/**
		 * \brief purges waiting queue
		 */
		bool purge() {
			uint64_t cT = BaseOrder<NetworkType>::pssi_->scheduler_.getTime();
			while (!queue_.empty()) {
				if (std::get<SEQNR>(queue_.top()) == nextRec_) {
					// next messages is available
					BaseOrder<NetworkType>::function_(std::get<BUFID>(queue_.top()), msgProcess::MSG_PROCESS_AND_DELETE);
					delivered_++;
					queue_.pop();
					nextRec_++;
				} else if (std::get<TIME>(queue_.top()) + Timeout < cT) {
					// Timeout
					nextRec_ = std::get<SEQNR>(queue_.top());
				} else if (!droppedQueue_.empty() && nextRec_ == droppedQueue_.top()) {
					// This messge will never arrive
					nextRec_++;
					droppedQueue_.pop();
				} else {
					// still waiting
					break;
				}
			}
			return true;
		}
		
		/**
		 * \brief Elements of the Tuple used to store arrived messages
		 */
		enum QueueTuple {
			SEQNR,
			BUFID,
			TIME
		};
	};

} /* namespace order */
} /* namespace pubsub */
} /* namespace m2etis */

#if I6E_PLATFORM == I6E_PLATFORM_WIN32
	#pragma warning(pop)
#endif

#endif /* __M2ETIS_PUBSUB_ORDER_MTPORDER_H__ */

/**
 *  @}
 */
