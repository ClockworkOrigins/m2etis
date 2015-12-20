/*
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
 * \addtogroup pubsub
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_DELIVER_ACKDELIVER_H__
#define __M2ETIS_PUBSUB_DELIVER_ACKDELIVER_H__

#include <string>
#include <queue>
#include <map>

#include "m2etis/message/info/deliver/AckDeliverInfo.h"

#include "m2etis/pubsub/MsgProcess.h"
#include "m2etis/pubsub/deliver/BaseDeliver.h"

#include "m2etis/util/SegmentTree.h"

namespace m2etis {
namespace pubsub {
namespace deliver {

	struct Comp {
		bool operator()(const std::pair<uint64_t, std::pair<uint64_t, int>> & a, const std::pair<uint64_t, std::pair<uint64_t, int>> & b) {
			return !(a < b);
		}
	};

	template<class NetworkType, int Retries, Amount Multiples>
	class AckDeliver : public BaseDeliver<NetworkType> {
	public:
		typedef message::AckDeliverInfo DeliverInfoType;

		AckDeliver(PubSubSystemEnvironment * pssi, const typename NetworkType::Key & self) : BaseDeliver<NetworkType>(pssi), queue_(), acked_(), delivered_() {
			periodicID_ = pssi->scheduler_.runRepeated(5000, boost::bind(&AckDeliver::periodicCheck, this), 1);
		}

		virtual ~AckDeliver() {
			BaseDeliver<NetworkType>::pssi_->scheduler_.stop(periodicID_);
		}

		/**
		 * \brief used to process payload
		 */
		bool processPublishPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			return process(ptr, sender);
		}

		/**
		 * \brief used to process payload
		 */
		bool processNotifyPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			return process(ptr, sender);
		}

		/**
		 * \brief used to process payload
		 */
		bool processSubscribePayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			return process(ptr, sender);
		}

		/**
		 * \brief used to process payload
		 */
		bool processUnsubscribePayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			return process(ptr, sender);
		}
		
		bool process(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			message::AckDeliverInfo::Ptr info = boost::static_pointer_cast<message::AckDeliverInfo>(ptr);
			DeliverInfoType::Ptr newInfo = boost::make_shared<DeliverInfoType>();
			newInfo->nr = info->nr;
			BaseDeliver<NetworkType>::sendCtrlMsg_(newInfo, sender, ControlTarget::SINGLE);
			if (delivered_[sender].contains(info->nr)) {
				if (Multiples == Amount::AT_LEAST_ONCE) {
					return true;
				} else {
					return false;
				}
			} else {
				delivered_[sender].insert(info->nr);
				return true;
			}
		}

		// same as other message, but always with strategy EXACTLY_ONCE
		bool processOtherControlPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			message::AckDeliverInfo::Ptr info = boost::static_pointer_cast<message::AckDeliverInfo>(ptr);
			DeliverInfoType::Ptr newInfo = boost::make_shared<DeliverInfoType>();
			newInfo->nr = info->nr;
			BaseDeliver<NetworkType>::sendCtrlMsg_(newInfo, sender, ControlTarget::SINGLE);
			if (delivered_[sender].contains(info->nr)) {
				return false;
			} else {
				delivered_[sender].insert(info->nr);
				return true;
			}
		}

		/**
		 * \brief creates the DeliverInfo
		 */
		void configureDeliverInfo(uint64_t id, const message::ActionType mtype, typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & receiver, message::ControlType ct) {
			if (ct != message::ControlType::DELIVER) {
				message::AckDeliverInfo::Ptr info = boost::static_pointer_cast<message::AckDeliverInfo>(ptr);
				info->nr = id;
				uint64_t cT = BaseDeliver<NetworkType>::pssi_->scheduler_.getTime();
				queue_.push(std::make_pair(cT + 100000, std::make_pair(id, 0)));
				BaseDeliver<NetworkType>::process_(id, msgProcess::MSG_PROCESS);
			} else {
				BaseDeliver<NetworkType>::process_(id, msgProcess::MSG_PROCESS_AND_DELETE);
			}
		}

		/**
		 * \brief processes Control Messages (id got acked)
		 */
		bool processControlPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			message::AckDeliverInfo::Ptr info = boost::static_pointer_cast<message::AckDeliverInfo>(ptr);
			acked_.insert(info->nr);
			return false;
		}

		/**
		 * \brief checks the queue for un-acked messages
		 */
		bool periodicCheck() {
			uint64_t cT = BaseDeliver<NetworkType>::pssi_->scheduler_.getTime();
			while (!queue_.empty() && queue_.top().first < cT) {
				// message wasn't acked
				std::set<uint64_t>::iterator it = acked_.find(queue_.top().second.first);
				if (it != acked_.end()) {
					acked_.erase(it);
					BaseDeliver<NetworkType>::process_(queue_.top().second.first, msgProcess::MSG_DELETE);
				} else {
					if (queue_.top().second.second < Retries) {
						BaseDeliver<NetworkType>::process_(queue_.top().second.first, msgProcess::MSG_PROCESS);
						queue_.push(std::make_pair(cT + 100000, std::make_pair(queue_.top().second.first, queue_.top().second.second + 1)));
					} else {
						BaseDeliver<NetworkType>::process_(queue_.top().second.first, msgProcess::MSG_DELETE);
					}
				}
				queue_.pop();
			}
			return true;
		}

	private:
		/**
		 * \brief stores all ids which has not been acked yet
		 */
		std::priority_queue<std::pair<uint64_t, std::pair<uint64_t, int>>, std::vector<std::pair<uint64_t, std::pair<uint64_t, int>>>, Comp> queue_;

		/**
		 * \brief stores all message ids that got acked
		 */
		std::set<uint64_t> acked_;

		std::map<typename NetworkType::Key, util::SegmentTree<uint64_t>> delivered_;

		uint64_t periodicID_;
	};

} /* namespace deliver */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_DELIVER_ACKDELIVER_H__ */

/**
 *  @}
 */
