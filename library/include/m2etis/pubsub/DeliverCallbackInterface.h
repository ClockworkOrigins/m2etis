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
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_DELIVERCALLBACKINTERFACE_H__
#define __M2ETIS_PUBSUB_DELIVERCALLBACKINTERFACE_H__

#include "m2etis/util/SystemParameters.h"

#include "m2etis/message/M2Message.h"

namespace m2etis {
namespace pubsub {

	/**
	 * \ingroup pubsub
	 *
	 * \class DeliverCallbackInterface
	 * \brief DeliverCallbackInterface
	 *
	 * Long Desc
	 *
	 */
	template<class EventType>
	class BasicDeliverCallbackInterface {
	public:
		/**
		 * \brief Destructor
		 */
		virtual ~BasicDeliverCallbackInterface() {}

		/**
		 * \brief deliver a callback
		 *
		 * \param[in] message delivered message
		 */
		virtual void deliverCallback(const typename m2etis::message::M2Message<EventType>::Ptr m) = 0;
	};

	M2ETIS_API typedef BasicDeliverCallbackInterface<std::vector<unsigned char> > DeliverCallbackInterface;

} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_DELIVERCALLBACKINTERFACE_H__ */

/*
 * @}
 */
