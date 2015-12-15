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

#ifndef __M2ETIS_PUBSUB_VALIDITY_TIMEVALIDITY_H__
#define __M2ETIS_PUBSUB_VALIDITY_TIMEVALIDITY_H__

#include <string>

#include "m2etis/message/info/validity/TimeValidityInfo.h"
#include "m2etis/pubsub/validity/BaseValidity.h"

namespace m2etis {
namespace pubsub {
namespace validity {

	template<uint64_t MIN, uint64_t MAX>
	class TimeValidity : public BaseValidity {
	public:
		typedef message::TimeValidityInfo ValidityInfoType;

		explicit TimeValidity(PubSubSystemEnvironment * pssi) : BaseValidity(pssi), _pssi(pssi) {}
		virtual ~TimeValidity() {}

		void configureValidityInfo(typename message::ValidityInfo::Ptr vInfo) {
			message::TimeValidityInfo::Ptr info = boost::static_pointer_cast<message::TimeValidityInfo>(vInfo);
			if (info->_timeStamp == UINT64_MAX) {
				info->_timeStamp = _pssi->clock_.getTime();
			}
		}

		void processPublishPayload(typename message::ValidityInfo::Ptr vInfo) {}

		void processNotifyPayload(typename message::ValidityInfo::Ptr vInfo) {}

		bool isValid(typename message::ValidityInfo::Ptr vInfo) {
			message::TimeValidityInfo::Ptr info = boost::static_pointer_cast<message::TimeValidityInfo>(vInfo);
			uint64_t cT = _pssi->clock_.getTime();
			if (info->_timeStamp < cT && (cT - info->_timeStamp > MAX || cT - info->_timeStamp < MIN)) {
				return false;
			}
			return true;
		}

	private:
		PubSubSystemEnvironment * _pssi;
	};

} /* namespace validity */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_VALIDITY_TIMEVALIDITY_H__ */

/**
 *  @}
 */
