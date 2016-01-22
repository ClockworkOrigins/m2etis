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
 * @ {
 */

#ifndef __M2ETIS_MESSAGE_DETMERGEORDERINFO_H__
#define __M2ETIS_MESSAGE_DETMERGEORDERINFO_H__

#include <cstdlib>

#include "m2etis/message/info/OrderInfo.h"
#include "m2etis/util/Logger.h"

#include "boost/serialization/base_object.hpp"
#include "boost/serialization/shared_ptr.hpp"

namespace m2etis {
namespace message {

	template<class Config>
	class DetMergeOrderInfo : public OrderInfo, public Config {
	public:
		typedef boost::shared_ptr<DetMergeOrderInfo> Ptr;

		/**
		 * \brief don't serialise this for SUB and UNSUB msgs
		 */
		static bool doSerialize(ActionType t) {
			if (t == SUBSCRIBE || t == UNSUBSCRIBE || t == JOIN || t == STATE || t == LEAVE) {
				return false;
			}
			return true;
		}

		/**
		 * \brief represents a timestamp
		 */
		typedef struct Timestamp {
			Timestamp() : kn(2 * Config::eps), unique(std::rand()), r(0), c(0) {}

			template <typename Archive>
			void serialize(Archive & ar, const unsigned int /*version*/) {
				ar & r;
				ar & c;
				ar & kn;
				ar & unique;
			}

			std::vector<int> kn; // kn.e_j
			int unique; // unique id (needed to get a pre-defined order with 2 equal Timestamps)
			uint64_t r; // physical clock on creator
			int64_t c; // difference to max clock

			/**
			 * \brief compares two structs
			 */
			bool operator<(const struct Timestamp & other) {
				if (r + c == other.r + other.c) {
					uint64_t e = Config::eps;
					size_t c1 = size_t(c), c2 = size_t(other.c);

					while (e != 0) {
						if (!(c1 < 2 * Config::eps && c2 < 2 * Config::eps)) {
							return false;
						}
						if (kn[c1] != other.kn[c2]) {
							return kn[c1] < other.kn[c2];
						}
						c1--;
						c2--;
						e--;
					}

					return unique < other.unique;
				} else {
					return r + c < other.r + other.c;
				}
			}

			/**
			 * \brief creates a readable string out o f a timestamp
			 */
			std::string toString() const {
				std::stringstream ss;
				ss << "TS: {r: " << r << ", c: " << c << ", kn: ";
				for (unsigned int t = 0; t < Config::eps * 2; ++t) {
					ss << kn[t] << ", ";
				}
				ss << "u: " << unique << "}";
				return ss.str();
			}
		} Timestamp;

		typedef boost::shared_ptr<Timestamp> timestamp_p;

		DetMergeOrderInfo() : ts(boost::make_shared<Timestamp>()) {}

		DetMergeOrderInfo(const DetMergeOrderInfo & other) : ts(boost::make_shared<Timestamp>(*other.ts)) {}

		/**
		 * \brief compares two orderinfos by their timestamp
		 */
		bool operator<(const DetMergeOrderInfo & other) const {
			return *(ts.get()) < *(other.ts.get());
		}

		/**
		 * \brief timestamp for this OrderInfo
		 */
		timestamp_p ts;
		
	private:
		friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive & ar, const unsigned int /*version*/) {
			ar & boost::serialization::base_object<OrderInfo>(*this);
			ar & ts;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_DETMERGEORDERINFO_H__ */

/**
 *  @}
 */
