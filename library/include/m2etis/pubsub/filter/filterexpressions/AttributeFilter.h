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
 * \addtogroup pubsub
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_FILTER__FILTEREXPRESSIONS_ATTRIBUTEFILTER_H__
#define __M2ETIS_PUBSUB_FILTER__FILTEREXPRESSIONS_ATTRIBUTEFILTER_H__

#include <functional>

#include "m2etis/pubsub/filter/filterexpressions/Predicate.h"
#include "m2etis/pubsub/filter/AttributeAccessor.h"

#include "boost/serialization/string.hpp"

namespace m2etis {
namespace pubsub {

	M2ETIS_API extern std::map<filter::AttributeName, std::shared_ptr<filter::AttributeAccessor_Basic> > attributes_accessor_map; // TODO: (Roland) make non-static, defined where?

namespace filter {

	template<typename EventType, typename AttributeType>
	class AttributeFilter : public Predicate<EventType> {
	public:
		AttributeFilter() : attribute_id_(-1) {}
		AttributeFilter(const AttributeName attribute_id, const std::vector<AttributeType> & constants): attribute_id_(attribute_id), constants_(constants) {}

		virtual ~AttributeFilter() {}

		virtual void getAttributeType(FilterVisitor<EventType> & visitor) const override {
			visitor.getAttributeType(this);
		}

		virtual bool match(const EventType & event) const override  {
			auto attribute_value_iterator =  attributes_accessor_map.find(attribute_id_);

			if (attribute_value_iterator == attributes_accessor_map.end()) {
				M2ETIS_THROW_API("AttributeFilter function match", "Attribute not found in attribute map.");
			}

			return matchAttribute((dynamic_cast<AttributeAccessor<EventType, AttributeType> *>(attribute_value_iterator->second.get()))->getAttributeValue(event));
		}

		virtual bool matchAttribute(const AttributeType & attribute_value) const = 0;

		// tests, whether 2 attribute filters overlap. needed to determine the trees to subscribe to by comparing dynamic and static predicates
		virtual bool overlaps(const Predicate<EventType> * other_predicate) const override {
			const AttributeFilter * other_attribute_filter = dynamic_cast<const AttributeFilter *>(other_predicate);

			if (other_attribute_filter == nullptr) {
				return true; // AttributeFilters for different attributes overlap
			}
			return overlaps(other_attribute_filter);
		};

		virtual bool overlaps(const AttributeFilter *) const = 0;

		template<typename OtherAttributeType>
		bool overlaps(const AttributeFilter<EventType, OtherAttributeType> *) const {
			return true; // filters with different attribute types overlap
		}

		const std::vector<AttributeType> get_constants() const { return constants_; }

		AttributeName get_attribute_id() const { return attribute_id_; } // needed for overlap calculation for filter attributes

	private:
		virtual bool doCompare(const FilterExp<EventType> & other_filter) const override {
			const AttributeFilter<EventType, AttributeType> * other_AttributeFilter = dynamic_cast<const AttributeFilter<EventType, AttributeType> *>(&other_filter);

			if (other_AttributeFilter) {
				return (typeid(*this) == typeid(*other_AttributeFilter)  && attribute_id_ == other_AttributeFilter->attribute_id_ && constants_ == other_AttributeFilter->constants_);
			} else {
				return false;
			}
		}

		virtual size_t doHash() const {
			// hash function should be improved:
			if (constants_.empty()) {
				return (std::hash<AttributeName>()(attribute_id_) ^ std::hash<std::string>()(std::string(typeid(*this).name())));
			} else {
				return ((std::hash<AttributeName>()(attribute_id_) ^ std::hash<AttributeType>()(*(constants_.begin()))) ^ std::hash<std::string>()(std::string(typeid(*this).name())));
			}
		}

		AttributeName attribute_id_;
		std::vector<AttributeType> constants_;

		friend class boost::serialization::access; // not sure if necessary
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<Predicate<EventType>>(*this);
			ar & attribute_id_;
			ar & constants_;
		}
	}; // class AttributeFilter

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER__FILTEREXPRESSIONS_ATTRIBUTEFILTER_H__ */

/**
 *  @}
 */
