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

#ifndef __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_TRUEPREDICATE_H__
#define __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_TRUEPREDICATE_H__

#include "m2etis/pubsub/filter/filterexpressions/Predicate.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	// class TruePredicate helpful if no static or dynamic predicate given
	template <typename EventType>
	class TruePredicate : public Predicate<EventType> {
	public:
		typedef EventType schema; // needed for operator overloading
		// TODO: (Roland) probably not needed, remove:
		virtual void Accept(FilterVisitor<EventType> & filter_visitor) const override {
			filter_visitor.Visit(this);
		}

		virtual bool match(const EventType & event) const { return true; }
		virtual bool overlaps(const Predicate<EventType> * other_predicate) const { return true; } // other_predicate assumed to be satisfiable
		virtual ~TruePredicate() {}

		virtual void getAttributeType(FilterVisitor<EventType> & visitor) const override {
			M2ETIS_THROW_API("TruePredicate", "getAttributeType not allowed in strategies with attribute types.");
		}

	private:
		friend class boost::serialization::access; // not sure if necessary
		template <typename Archive> void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<Predicate<EventType> >(*this);
		}
	}; // class TruePredicate

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_TRUEPREDICATE_H__ */
