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
 * \addtogroup message
 * @{
 */

#ifndef __M2ETIS_MESSAGE_DIRECTBROADCASTROUTINGINFO_H__
#define __M2ETIS_MESSAGE_DIRECTBROADCASTROUTINGINFO_H__

#include "m2etis/message/info/RoutingInfo.h"

#include "boost/serialization/base_object.hpp"
#include "boost/serialization/set.hpp"

namespace m2etis {
namespace message {

	template<class NetworkType>
	class DirectBroadcastRoutingInfo : public RoutingInfo<NetworkType> {
	public:
		typedef boost::shared_ptr<DirectBroadcastRoutingInfo<NetworkType>> Ptr;

        static bool doSerialize(ActionType t) {
        	if (t == NOTIFY || t == PUBLISH || t == JOIN || t == STATE || t == LEAVE) {
        		return false;
        	}

        	return true;
        }

		/* serialized variables */
		std::set<typename NetworkType::Key> _nodes;

	private:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<RoutingInfo<NetworkType>>(*this);
			ar & _nodes;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_DIRECTBROADCASTROUTINGINFO_H__ */

/**
 * @}
 */
