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

#ifndef __M2ETIS_PUBSUB_FILTER_ATTRIBUTEACCESSOR_H__
#define __M2ETIS_PUBSUB_FILTER_ATTRIBUTEACCESSOR_H__

#include "m2etis/pubsub/filter/AttributeAccessorBasic.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	// maps attribute identifier for event type and attribute type to attribute value
	template<typename EventType, typename AttributeType>
	class AttributeAccessor : public AttributeAccessor_Basic  {
	public:
		// constructor with pointer to EventType attribute member
		explicit AttributeAccessor(AttributeType EventType::*attribute_accessor_pointer) : attribute_accessor_pointer_(attribute_accessor_pointer) {}

		// constructor with get-function for EventType attribute member
		explicit AttributeAccessor(std::function<AttributeType(const EventType &)> attribute_accessor_function) : attribute_accessor_function_(attribute_accessor_function), attribute_accessor_pointer_(nullptr) {}

		AttributeType getAttributeValue(const EventType & event) {
			return attribute_accessor_pointer_ ? event.*attribute_accessor_pointer_ : attribute_accessor_function_(event);
		}

	private:
		std::function<AttributeType(const EventType &)> attribute_accessor_function_; // returns Attribute value for attribute id

		AttributeType EventType::*attribute_accessor_pointer_;
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_ATTRIBUTEACCESSOR_H__ */

/**
 *  @}
 */
