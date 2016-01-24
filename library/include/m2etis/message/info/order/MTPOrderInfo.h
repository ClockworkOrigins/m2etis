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

#ifndef __M2ETIS_MESSAGE_MTPORDERINFO_H__
#define __M2ETIS_MESSAGE_MTPORDERINFO_H__

#include "m2etis/message/info/OrderInfo.h"

#include "boost/serialization/base_object.hpp"

namespace m2etis {
namespace message {

	class MTPOrderInfo : public OrderInfo {
	public:
		typedef boost::shared_ptr<MTPOrderInfo> Ptr;

		enum MTPStatus {
			STAT_UNDEFINED,
			STAT_PENDING,
			STAT_ACCEPTED,
			STAT_REJECTED
		};

		enum MTPType {
			TYPE_UNDEFINED,
			TYPE_TOKEN_REQUEST,
			TYPE_TOKEN_GRANT,
			TYPE_PUBLISH
		};

		static bool doSerialize(ActionType t) {
			if (t == SUBSCRIBE || t == UNSUBSCRIBE || t == JOIN || t == STATE || t == LEAVE) {
				return false;
			}

			return true;
		}

		MTPOrderInfo() : type_(TYPE_UNDEFINED), seqNr(UINT64_MAX), missing_() {}

		MTPType type_;

		uint64_t seqNr;
		
		/**
		 * \brief stores information about dropped messages
		 */
		std::vector<uint64_t> missing_;

	private:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<OrderInfo>(*this);
			ar & type_;
			ar & seqNr;
			ar & missing_;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_MTPORDERINFO_H__ */

/**
 * @}
 */
