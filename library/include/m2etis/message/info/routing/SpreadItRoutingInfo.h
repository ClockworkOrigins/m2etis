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

#ifndef __M2ETIS_MESSAGE_SPREADITROUTINGINFO_H__
#define __M2ETIS_MESSAGE_SPREADITROUTINGINFO_H__

#include "m2etis/message/info/RoutingInfo.h"

#include "boost/serialization/base_object.hpp"

namespace m2etis {
namespace message {

	template<class NetworkType>
	class SpreadItRoutingInfo : public RoutingInfo<NetworkType> {
	public:
		typedef boost::shared_ptr<SpreadItRoutingInfo<NetworkType>> Ptr;

        static bool doSerialize(ActionType t) {
        	if (t == NOTIFY || t == PUBLISH || t == JOIN || t == STATE || t == LEAVE) {
        		return false;
        	}

        	return true;
        }

		/* serialized variables */
		typename NetworkType::Key node_adress;

	private:
		friend class boost::serialization::access;

		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<RoutingInfo<NetworkType>>(*this);
			ar & node_adress;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_SPREADITROUTINGINFO_H__ */

/**
 * @}
 */
