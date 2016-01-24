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
 * @{
 */

#ifndef __M2ETIS_PUBSUB_FILTER_FILTEVISITORS_FILTERVISITOR_H__
#define __M2ETIS_PUBSUB_FILTER_FILTEVISITORS_FILTERVISITOR_H__

namespace m2etis {
namespace pubsub {
namespace filter {

	template <typename EventType> class FilterExp;
	template <typename EventType> class AndExp;
	template <typename EventType> class OrExp;
	template <typename EventType> class Predicate;
	template <typename EventType, typename AttributeType> class GreaterThanAttributeFilter;
	template <typename EventType, typename AttributeType> class LessThanAttributeFilter;
	template <typename EventType, typename AttributeType> class EqualsAttributeFilter;
	template <typename EventType, typename AttributeType> class NotEqualsAttributeFilter;
	template <typename EventType, typename AttributeType> class AttributeFilter;

	template <typename EventType>
	class FilterVisitor {
	public:
		virtual ~FilterVisitor() {}

		virtual void Visit(const FilterExp<EventType> *) = 0;
		virtual void Visit(const AndExp<EventType> *) = 0;
		virtual void Visit(const OrExp<EventType> *) = 0;
		virtual void Visit(const Predicate<EventType> *) = 0;

		virtual void getAttributeType(const GreaterThanAttributeFilter<EventType, std::string> *)  {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType with const GreaterThanAttributeFilter<EventType, std::string> * called.");
		}

		virtual void getAttributeType(const GreaterThanAttributeFilter<EventType, int> *)  {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType with const GreaterThanAttributeFilter<EventType, std::int> * called.");
		}

		virtual void getAttributeType(const LessThanAttributeFilter<EventType, std::string> *)  {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType called.");
		}

		virtual void getAttributeType(const LessThanAttributeFilter<EventType, int> *)  {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType called.");
		}

		virtual void getAttributeType(const EqualsAttributeFilter<EventType, std::string> *)  {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType called.");
		}

		virtual void getAttributeType(const EqualsAttributeFilter<EventType, int> *)  {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType called.");
		}

		virtual void getAttributeType(const NotEqualsAttributeFilter<EventType, std::string> *)  {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType called.");
		}

		virtual void getAttributeType(const NotEqualsAttributeFilter<EventType, int> *)  {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType called.");
		}

		virtual void getAttributeType(const AttributeFilter<EventType, std::string> *) {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType with const AttributeFilter<EventType, std::string> * called.");
		}

		virtual void getAttributeType(const AttributeFilter<EventType, int> *) {
			M2ETIS_THROW_API("FilterVisitor", "unimplemented method getAttributeType with const AttributeFilter<EventType, int> * called.");
		}

	protected:
		FilterVisitor() {}
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTEVISITORS_FILTERVISITOR_H__ */

/**
 * @}
 */
