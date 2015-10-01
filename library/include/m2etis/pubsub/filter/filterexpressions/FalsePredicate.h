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

#ifndef __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_FALSEPREDICATE_H__
#define __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_FALSEPREDICATE_H__

#include "m2etis/pubsub/filter/filterexpressions/Predicate.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	// class FalsePredicate helpful to avoid redundancies when using periodic resubusbecie
	// maybe not useful any more
	template <typename EventType>
	class FalsePredicate : public Predicate<EventType> {
	public:
		typedef EventType schema; // needed for operator overloading

		virtual void Accept(FilterVisitor<EventType> & filter_visitor) const override {
			filter_visitor.Visit(this);
		}

		virtual bool match(const EventType& event) const { return false; }

		virtual bool overlaps(const Predicate<EventType> * other_predicate) const { return false; }

		virtual ~FalsePredicate() {}

	private:
		friend class boost::serialization::access; // not sure if necessary
		template <typename Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<Predicate<EventType> >(*this);
		}
	}; // class FalsePredicate

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_FALSEPREDICATE_H__ */
