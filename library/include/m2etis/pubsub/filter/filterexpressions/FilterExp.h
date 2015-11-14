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

#ifndef __M2ETIS_PUBSUB_FILTER_FILTEREXP_H__
#define __M2ETIS_PUBSUB_FILTER_FILTEREXP_H__

#include "m2etis/pubsub/filter/filtervisitors/FilterVisitor.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType>
	class FilterExp {
	public:
		typedef boost::shared_ptr<FilterExp<EventType>> FilterExpPtr;
		FilterExp() {}
		virtual ~FilterExp() {}

		virtual void Accept(FilterVisitor<EventType> & filter_visitor) const {
			filter_visitor.Visit(this);
		}

		// comparison operators to avoid having global ids in the network to deregister individual filters:
		bool operator==(const FilterExp & other_filter) const {
			return this->doCompare(other_filter) && other_filter.doCompare(*this);
		}

		bool operator!=(const FilterExp & other_filter) const {
			return !(*this == other_filter);
		}

		// hash function to use predicates as keys in (unordered_)maps
		// see below regarding specializing function in  std namespace
		size_t hash() const {
			return doHash();
		}

		// for debugging purposes:
		virtual operator std::string() const {
			return typeid(*this).name();
		};
		
		std::string toString() {
			return "FilterExpr";
		}

	private:
		virtual bool doCompare(const FilterExp &) const { return false; } // TODO: (Roland) make abstract

		virtual size_t doHash() const { return 0; } // TODO: (Roland) make abstract

		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive &, const unsigned int) {
		}
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

namespace std {

    template<typename EventType>
    struct hash<m2etis::pubsub::filter::FilterExp<EventType>> : public unary_function<m2etis::pubsub::filter::FilterExp<EventType>, size_t> {
        size_t operator()(const m2etis::pubsub::filter::FilterExp<EventType> & filter) const {
            return filter.hash();
        }
    };

} /* namespace std */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTEREXP_H_ */

/**
 *  @}
 */
