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

#ifndef __M2ETIS_MESSAGE_NACKDELIVERINFO_H__
#define __M2ETIS_MESSAGE_NACKDELIVERINFO_H__

#include "boost/shared_ptr.hpp"

namespace m2etis {
namespace message {

	class NackDeliverInfo : public DeliverInfo {
	public:
		typedef boost::shared_ptr<NackDeliverInfo> Ptr;

		NackDeliverInfo() : DeliverInfo(), nr(), dropped() {}

		virtual ~NackDeliverInfo() {}

		static bool doSerialize(const ActionType) {
			return true;
		}

		uint64_t nr;

		std::vector<uint64_t> dropped;

		template<typename Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & nr;
			ar & dropped;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_NACKDELIVERINFO_H__ */

/**
 * @}
 */
