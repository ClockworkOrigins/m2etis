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

#ifndef __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_NOTEQUALSATTRIBUTEFILTER_H__
#define __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_NOTEQUALSATTRIBUTEFILTER_H__

#include "m2etis/pubsub/filter/filterexpressions/AttributeFilter.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename AttributeType> class EqualsAttributeFilter;
	template<typename EventType, typename AttributeType> class GreaterThanAttributeFilter;
	template<typename EventType, typename AttributeType> class LessThanAttributeFilter;

	template<typename EventType, typename AttributeType>
	class NotEqualsAttributeFilter : public AttributeFilter<EventType, AttributeType> {
	public:
		typedef EventType schema; // needed for operator overloading
		NotEqualsAttributeFilter() : AttributeFilter<EventType, AttributeType>(-1, {}) {} // needed for boost serialization
		NotEqualsAttributeFilter(const AttributeName attribute_id, const AttributeType & constants) : AttributeFilter<EventType, AttributeType>(attribute_id, {constants}) {}
		virtual ~NotEqualsAttributeFilter() {}

		virtual void getAttributeType(FilterVisitor<EventType> & visitor) const override {
			visitor.getAttributeType(this);
		}

		// function called to match against an event
		virtual bool matchAttribute(const AttributeType & attribute) const override {
			return attribute != this->get_constants()[0]; // attribute type has to implement "!="-operator
		}

		using AttributeFilter<EventType, AttributeType>::overlaps;

		virtual bool overlaps(const AttributeFilter<EventType, AttributeType> * other_filter) const override {
			if (this->get_attribute_id() != other_filter->get_attribute_id()) {
				return true; // AttributeFilters for different attributes overlap
			}

			if (typeid(EqualsAttributeFilter<EventType, AttributeType>) == typeid(*other_filter)) {
				return other_filter->overlaps(this);
			}
			if (typeid(NotEqualsAttributeFilter<EventType, AttributeType>) == typeid(*other_filter)) {
				return this->get_constants()[0] == other_filter->get_constants()[0];
			}
			if (typeid(GreaterThanAttributeFilter<EventType, AttributeType>) == typeid(*other_filter)) {
				return true; // TODO: (Roland) check for numeric_limits
			}
			if (typeid(LessThanAttributeFilter<EventType, AttributeType>) == typeid(*other_filter)) {
				return true;
			}

			return true;
		}

	 private:
		friend class boost::serialization::access; // not sure if necessary
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
	  		ar & boost::serialization::base_object<AttributeFilter<EventType, AttributeType>>(*this);
		}
	}; // NotEqualsAttributeFilter

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_NOTEQUALSATTRIBUTEFILTER_H__ */
