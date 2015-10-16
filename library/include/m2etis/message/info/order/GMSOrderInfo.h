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

#ifndef __M2ETIS_MESSAGE_GMSORDERINFO_H__
#define __M2ETIS_MESSAGE_GMSORDERINFO_H__

#include "m2etis/message/info/OrderInfo.h"

#include "boost/serialization/base_object.hpp"

namespace m2etis {
namespace message {

	enum class TYPE {
		NONE,
		UNSUB
	};

	template<class NetworkType>
	class GMSOrderInfo : public OrderInfo {
	public:
		typedef boost::shared_ptr<GMSOrderInfo> Ptr;

		GMSOrderInfo() : realTree(UINT64_MAX), msgNr(0), seqNr(0), mT(0), type(TYPE::NONE), sender() {
		}

		/**
		 * \brief serialise for ALL messages
		 */
		static bool doSerialize(ActionType) {
				return true;
		}

		/**
		 * \brief id on which the real Msg was published
		 */
		uint64_t realTree = UINT64_MAX;

		/**
		 * \brief globally unique msg id set by root
		 */
		uint64_t msgNr = 0;

		/**
		 * \brief id used to order msgs on a single link
		 */
		uint64_t seqNr = 0;

		/**
		 * \brief main tree for sender
		 */
		uint64_t mT = 0;

		/**
		 * \brief type of this message
		 */
		TYPE type;

		/**
		 * \brief initial sender of this msg
		 */
		typename NetworkType::Key sender;

	private:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<OrderInfo>(*this);
			ar & msgNr;
			ar & realTree;
			ar & seqNr;
			ar & mT;
			ar & type;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_GMSORDERINFO_H__ */
