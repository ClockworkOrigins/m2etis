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


#ifndef __M2ETIS_PUBSUB_DELIVER_NACKDELIVER_H__
#define __M2ETIS_PUBSUB_DELIVER_NACKDELIVER_H__

#include <string>
#include <queue>
#include <map>

#include "m2etis/message/info/deliver/NackDeliverInfo.h"

#include "m2etis/pubsub/MsgProcess.h"
#include "m2etis/pubsub/deliver/BaseDeliver.h"

namespace m2etis {
namespace pubsub {
namespace deliver {

	template<class NetworkType, int Retries, Amount Multiples>
	class NackDeliver : public BaseDeliver<NetworkType> {
	public:
		typedef message::NackDeliverInfo DeliverInfoType;

		NackDeliver(PubSubSystemEnvironment * pssi, const typename NetworkType::Key & self) : BaseDeliver<NetworkType>(pssi), buffer_(), missing_(), lastMessages_(), lastID_(), self_(self), delivered_() {
			periodicID_ = pssi->scheduler_.runRepeated(REQUESTTIME / 2, boost::bind(&NackDeliver::periodicCheck, this), 1);
		}

		virtual ~NackDeliver() {
			BaseDeliver<NetworkType>::pssi_->scheduler_.stop(periodicID_);
		}

		/**
		 * \brief used to process payload
		 */
		bool processPublishPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			return processPublishAndNotify(ptr, sender);
		}

		/**
		 * \brief used to process payload
		 */
		bool processNotifyPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			return processPublishAndNotify(ptr, sender);
		}

		/**
		 * \brief used to process payload
		 */
		bool processSubscribePayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			return processPublishAndNotify(ptr, sender);
		}

		/**
		 * \brief used to process payload
		 */
		bool processUnsubscribePayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			return processPublishAndNotify(ptr, sender);
		}

		bool processOtherControlPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			processPublishAndNotify(ptr, sender);
			return true; // FIXME: (Daniel) think of this, may be stupid because we only want every Control Message once
		}

		/**
		 * \brief creates the DeliverInfo
		 */
		void configureDeliverInfo(uint64_t id, const message::ActionType mtype, typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & receiver, message::ControlType ct) {
			if (ct != message::ControlType::DELIVER) {
				message::NackDeliverInfo::Ptr info = boost::static_pointer_cast<message::NackDeliverInfo>(ptr);
				info->nr = lastID_[receiver]++;
				info->dropped = dropped_[receiver];
				dropped_[receiver].clear();
				buffer_.insert(std::make_pair(std::make_pair(self_, info->nr), std::make_pair(id, BaseDeliver<NetworkType>::pssi_->scheduler_.getTime() + BUFFERTIME)));
				BaseDeliver<NetworkType>::process_(id, msgProcess::MSG_PROCESS);
			} else {
				BaseDeliver<NetworkType>::process_(id, msgProcess::MSG_PROCESS_AND_DELETE);
			}
		}

		/**
		 * \brief processes Control Messages (id got acked)
		 */
		bool processControlPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			message::NackDeliverInfo::Ptr info = boost::static_pointer_cast<message::NackDeliverInfo>(ptr);
			typename std::map<std::pair<typename NetworkType::Key, uint64_t>, std::pair<uint64_t, uint64_t> >::iterator it = buffer_.find(std::make_pair(self_, info->nr));
			if (it != buffer_.end()) {
				it->second.second = BaseDeliver<NetworkType>::pssi_->scheduler_.getTime() + BUFFERTIME; // reset timeout for this message
				BaseDeliver<NetworkType>::process_(it->second.first, msgProcess::MSG_PROCESS); // and resend it
			} else {
				dropped_[sender].push_back(info->nr);
			}
			return false;
		}

	private:
		bool processPublishAndNotify(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) {
			message::NackDeliverInfo::Ptr info = boost::static_pointer_cast<message::NackDeliverInfo>(ptr);
			typename std::map<typename NetworkType::Key, uint64_t>::iterator it = lastMessages_.find(sender);
			bool ret = false;
			if (it != lastMessages_.end()) {
				if (it->second > info->nr) { // message received is either duplicate or older unreceived message
					if (missing_[sender].find(info->nr) == missing_[sender].end()) { // message was already received
						if (Multiples == Amount::AT_LEAST_ONCE) {
							ret = true;
						} else {
							ret = false;
						}
					} else { // message is received first time
						missing_[sender].erase(info->nr);
						ret = true;
					}
				} else if (it->second < info->nr) { // a definitly new message arrived
					typename std::map<typename NetworkType::Key, std::map<uint64_t, std::pair<uint64_t, int>> >::iterator it2 = missing_.find(sender);
					for (uint64_t i = it->second + 1; i < info->nr; ++i) {
						it2->second[i] = std::make_pair(BaseDeliver<NetworkType>::pssi_->scheduler_.getTime() + REQUESTTIME / 2, 0); // add all IDs between the last this node received and the one actually received
					}
					it->second = info->nr;
					ret = true;
				} else { // the current message arrives again
					if (Multiples == Amount::AT_LEAST_ONCE) {
						ret = true;
					} else {
						ret = false;
					}
				}
			} else {
				lastMessages_[sender] = info->nr;
				std::map<uint64_t, std::pair<uint64_t, int>> missings;
				for (uint64_t i = 0; i < info->nr; ++i) {
					missings[i] = std::make_pair(BaseDeliver<NetworkType>::pssi_->scheduler_.getTime() + REQUESTTIME / 2, 0); // add all IDs between the last this node received and the one actually received
				}
				missing_[sender] = missings;
				ret = true;
			}
			for (uint64_t p : info->dropped) {
				if (p < lastMessages_[sender]) {
					missing_[sender].erase(p);
				} else {
					for (uint64_t i = lastMessages_[sender] + 1; i < p; ++i) {
						missing_[sender][i] = std::make_pair(BaseDeliver<NetworkType>::pssi_->scheduler_.getTime() + REQUESTTIME / 2, 0); // add all IDs between the last this node received and the one actually received
					}
					lastMessages_[sender] = p;
				}
			}
			return ret;
		}

		/**
		 * \brief checks the queue for un-acked messages
		 */
		bool periodicCheck() {
			uint64_t cT = BaseDeliver<NetworkType>::pssi_->scheduler_.getTime();
			for (typename std::map<typename NetworkType::Key, std::map<uint64_t, std::pair<uint64_t, int>> >::iterator it = missing_.begin(); it != missing_.end(); ++it) {
				for (typename std::map<uint64_t, std::pair<uint64_t, int>>::iterator it2 = it->second.begin(); it2 != it->second.end();) {
					if (it2->second.first < cT) {
						if (it2->second.second < Retries || Multiples == Amount::EXACTLY_ONCE) {
							typename DeliverInfoType::Ptr newInfo = boost::make_shared<DeliverInfoType>();
							newInfo->nr = it2->first;
							BaseDeliver<NetworkType>::sendCtrlMsg_(newInfo, it->first, ControlTarget::SINGLE);
							it2->second.second++;
							it2->second.first = cT + REQUESTTIME;
							it2++;
						} else {
							typename std::map<uint64_t, std::pair<uint64_t, int>>::iterator itDelete = it2;
							it2++;
							it->second.erase(itDelete);
						}
					} else {
						it2++;
					}
				}
			}
			for (typename std::map<std::pair<typename NetworkType::Key, uint64_t>, std::pair<uint64_t, uint64_t> >::iterator it = buffer_.begin(); it != buffer_.end();) {
				if (it->second.second < cT) {
					BaseDeliver<NetworkType>::process_(it->second.first, msgProcess::MSG_DELETE); // remove message from buffer
					typename std::map<std::pair<typename NetworkType::Key, uint64_t>, std::pair<uint64_t, uint64_t> >::iterator itDelete = it;
					++it;
					buffer_.erase(itDelete);
				} else {
					++it;
				}
			}
			return true;
		}

		/**
		 * \brief stores all ids which have to be stored until they should be delivered
		 */
		std::map<std::pair<typename NetworkType::Key, uint64_t>, std::pair<uint64_t, uint64_t> > buffer_;

		std::map<typename NetworkType::Key, std::map<uint64_t, std::pair<uint64_t, int>> > missing_;

		std::map<typename NetworkType::Key, uint64_t> lastMessages_;

		std::map<typename NetworkType::Key, std::vector<uint64_t>> dropped_;

		std::map<typename NetworkType::Key, uint64_t> lastID_;

		typename NetworkType::Key self_;

		uint64_t periodicID_;

		util::SegmentTree<uint64_t> delivered_;

		const uint64_t REQUESTTIME = 100000;

		const uint64_t BUFFERTIME = 10000000;
	};

} /* namespace deliver */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_DELIVER_NACKDELIVER_H__ */
