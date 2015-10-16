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

#ifndef __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_PREDICATE_H__
#define __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_PREDICATE_H__

#include "m2etis/pubsub/filter/filterexpressions/FilterExp.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType> class AttributeTypeInformation;
	template<typename EventType, typename NetworkType> class DecisionTreeFilter;
	template<typename EventType, typename NetworkType> class GeneralBooleanExpressionsFilter;

	template<typename EventType>
	class Predicate : public FilterExp<EventType> {
	public:
		virtual ~Predicate() {}
		virtual void Accept(FilterVisitor<EventType> & filter_visitor) const override {
			filter_visitor.Visit(this);
		}
		// function needed to determine the attribute type, e.g. in DecisionTreePreProcessVisitor

		virtual void getAttributeType(FilterVisitor<EventType> & visitor) const = 0;

		virtual bool match(const EventType &) const = 0; // TODO: (Roland) make abstract
		virtual bool overlaps(const Predicate<EventType> *) const { return true; } // implemented for serialization= 0;

	private:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<FilterExp<EventType>>(*this);
		}
	}; // class Predicate

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_PREDICATE_H__ */
