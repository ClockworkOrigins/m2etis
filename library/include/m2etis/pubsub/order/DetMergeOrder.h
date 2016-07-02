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

#ifndef __M2ETIS_PUBSUB_ORDER_DETMERGEORDER_H__
#define __M2ETIS_PUBSUB_ORDER_DETMERGEORDER_H__

#include <queue>

#include "m2etis/pubsub/order/BaseOrder.h"

#include "m2etis/message/info/order/DetMergeOrderInfo.h"

namespace m2etis {
namespace pubsub {
namespace order {

	/**
	 * \brief Implements the Deterministic Merge Order Strategy
	 */
	template<class NetworkType, class Config>
	class DetMergeOrder : public BaseOrder<NetworkType>, public Config {
		/**
		 * \brief sorts OrderInfos
		 */
		class Compare {
		public:
			bool operator()(const std::pair<typename message::DetMergeOrderInfo<Config>::Ptr, uint64_t> & a, const std::pair<typename message::DetMergeOrderInfo<Config>::Ptr, uint64_t> & b) const {
				// reveresed order to get the smallest pair first (priority_queue sorts maximum first)
				return !(*(a.first.get()) < *(b.first.get()));
			}
		};

	public:
		typedef message::DetMergeOrderInfo<Config> OrderInfoType;

		/**
		 * \brief Constructor
		 */
		DetMergeOrder(PubSubSystemEnvironment * pssi, bool b) : BaseOrder<NetworkType>(pssi, b), rt(0), lastEv(new typename OrderInfoType::Timestamp()), queue_(), pssi_(pssi) {
			lastEv->kn[0] = 1;
			periodicID_ = pssi->scheduler_.runRepeated(5000, std::bind(&DetMergeOrder::updatePhyClock, this), 1);
		}

		/**
		 * \brief Destructor
		 */
		virtual ~DetMergeOrder() {
			pssi_->scheduler_.stop(periodicID_);
		}

		/**
		 * \brief returns whether there are messages waiting
		 */
		bool hasPending() {
			return !queue_.empty();
		}

		/**
		 * \brief configure Infos
		 * This functions sets all necessary information for this message
		 */
		bool configureOrderInfo(uint64_t id, const message::ActionType mtype, typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key &) {
			if (mtype != message::PUBLISH) {
				BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
				return true;
			}
			typename message::DetMergeOrderInfo<Config>::Ptr info = boost::static_pointer_cast<message::DetMergeOrderInfo<Config>>(ptr);
			// update local time
			rt = pssi_->scheduler_.getTime() / Config::raster;
			lastEv->c = std::max(int64_t(0), int64_t(lastEv->r + lastEv->c - rt));
			// shifts kn with (rt - lastEv->r) -> this time elapsed since last update
			for (uint32_t t = 0; t < Config::eps * 2; ++t) {
				if (t + rt - lastEv->r < 2 * Config::eps) {
					lastEv->kn[t] = lastEv->kn[t + size_t(rt - lastEv->r)];
				} else {
					lastEv->kn[t] = 0;
				}
			}
			lastEv->kn[0]++;
			lastEv->r = rt;
			info->ts->r = lastEv->r;
			info->ts->c = lastEv->c;
			info->ts->kn = lastEv->kn;
			BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
			return true;
		}

		/**
		 * \brief process Control Messages
		 * They are only used to sync
		 */
		bool processControlPayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
			assert(false);
			return false;
		}

		/**
		 * \brief this strategie doesn't care about subscribes
		 */
		void processSubscribePayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief this strategie doesn't care about publish payloads here
		 */
		void processPublishPayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief called for every NotifyMsg that arrived
		 */
		void processNotifyPayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief this strategie doesn't need the other trees
		 */
		void otherOrders(const std::vector<DetMergeOrder *> &) {
		}

		/**
		 * \brief called when a message is received
		 * This function updates the internal state of this strategie and pushes the msg in a queue
		 */
		void receive(uint64_t id, typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key &) {
			typename OrderInfoType::Ptr info = boost::static_pointer_cast<OrderInfoType>(ptr);
			// Update time
			rt = pssi_->scheduler_.getTime() / Config::raster;
			lastEv->c = std::max(int64_t(0), std::max(int64_t(lastEv->r + lastEv->c - rt), int64_t(int64_t(info->ts->r) + info->ts->c - rt)));
			// shift array
			for(uint32_t t = 0; t < Config::eps * 2; ++t) {
				int32_t v1 = 0;
				int32_t v2 = 0;
				int32_t v3 = 0;
				// values outside the array are 0
				if (t + rt - lastEv->r < 2 * Config::eps && t + rt - lastEv->r >= 0) {
					v2 = lastEv->kn[t + size_t(rt - lastEv->r)];
				}
				if (t + rt - info->ts->r < 2 * Config::eps && t + rt - info->ts->r >= 0) {
					v3 = info->ts->kn[t + size_t(rt - info->ts->r)];
				}
				lastEv->kn[t] = max(v1, max(v2, v3));
			}
			lastEv->kn[0]++;
			lastEv->r = rt;
			queue_.push(std::make_pair(info, id));
		}

		// update clock and check for delivery
		bool updatePhyClock() {
			uint64_t a = pssi_->scheduler_.getTime();
			rt = a / Config::raster;
			// deliver all waiting msgs
			while (!queue_.empty() && queue_.top().first->ts->r + queue_.top().first->ts->c + Config::delta + Config::eps <= rt) {
				// delivers message
				BaseOrder<NetworkType>::function_(queue_.top().second, msgProcess::MSG_PROCESS_AND_DELETE);
				queue_.pop();
			}
			// reschedule this task
			return true;
		}

		/**
		 * \brief notified when message dropped by filter or validity strategy
		 */
		void notifyRemovedMessage(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

	private:
		template<class T>
		T max(const T & a, const T & b) {
			if (a > b) {
				return a;
			} else {
				return b;
			}
		}

		/**
		 * \brief current value of own clock
		 * This is the clock value used for this strategie. It's value is the real clock value
		 * divided by the Config::Raster to get a less precise value and save some space
		 */
		volatile uint64_t rt;

		typename OrderInfoType::timestamp_p lastEv; // <r.j, c.j, kn.j>

		/**
		 * \brief Queue containing all waiting messages, sorted by deliver time
		 */
		std::priority_queue<std::pair<typename OrderInfoType::Ptr, uint64_t>, std::vector<std::pair<typename OrderInfoType::Ptr, uint64_t>>, Compare> queue_;

		/**
		 * \brief pointer to the PSSE
		 */
		PubSubSystemEnvironment * pssi_;

		uint64_t periodicID_;
	};

} /* namespace order */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ORDER_DETMERGEORDER_H__ */

/**
 * @}
 */
