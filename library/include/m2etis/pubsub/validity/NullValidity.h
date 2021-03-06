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

#ifndef __M2ETIS_PUBSUB_VALIDITY_NULLVALIDITY_H__
#define __M2ETIS_PUBSUB_VALIDITY_NULLVALIDITY_H__

#include "m2etis/message/info/validity/NullValidityInfo.h"
#include "m2etis/pubsub/validity/BaseValidity.h"

namespace m2etis {
namespace pubsub {
namespace validity {

	class NullValidity : public BaseValidity {
	public:
		typedef message::NullValidityInfo ValidityInfoType;

		explicit NullValidity(PubSubSystemEnvironment * pssi) : BaseValidity(pssi) {}
		virtual ~NullValidity() {}

		void configureValidityInfo(message::ValidityInfo::Ptr vInfo) {}

		void processPublishPayload(message::ValidityInfo::Ptr vInfo) {}

		void processNotifyPayload(message::ValidityInfo::Ptr vInfo) {}

		bool isValid(message::ValidityInfo::Ptr vInfo) {
			return true;
		}
	};

} /* namespace validity */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_VALIDITY_NULLVALIDITY_H__ */

/**
 * @}
 */
