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

#ifndef __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_ANDEXP_H__
#define __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_ANDEXP_H__

#include <functional>

#include "m2etis/pubsub/filter/filterexpressions/FilterExp.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType>
	class AndExp : public FilterExp<EventType> {
	public:
		typedef EventType schema; // needed for operator overloading
		AndExp(const boost::shared_ptr<FilterExp<EventType>> op1, const boost::shared_ptr<FilterExp<EventType>> op2) : operand1_(op1), operand2_(op2) {}
		virtual ~AndExp() {}
		AndExp() {} // for boost serialization

		virtual void Accept(FilterVisitor<EventType> & filter_visitor) const override {
			operand1_->Accept(filter_visitor);
			operand2_->Accept(filter_visitor);
			filter_visitor.Visit(this);
		}

		virtual operator boost::shared_ptr<FilterExp<EventType>>() const {
			return boost::make_shared<AndExp<EventType>>(*this);
		}

		// for debugging purposes:
		virtual operator std::string() const override {
			return (std::string("AndExp") + "(\n" + std::string(*operand1_) + ", \t\t  " + std::string(*operand2_) + ")");
		}

	private:
		boost::shared_ptr<FilterExp<EventType>> operand1_;
		boost::shared_ptr<FilterExp<EventType>> operand2_;

		virtual bool doCompare(const FilterExp<EventType> & other_filter) const override {
			const AndExp * other_AndExp = dynamic_cast<const AndExp *>(&other_filter);

			if (other_AndExp) {
				return (*operand1_ == *(other_AndExp->operand1_) && *operand2_ == *(other_AndExp->operand2_));
			} else {
				return false;
			}
		}

		virtual size_t doHash() const {
			return std::hash<FilterExp<EventType>>()(*operand1_) ^ std::hash<FilterExp<EventType>>()(*operand2_);
		}

		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<FilterExp<EventType>>(*this);
			ar & operand1_;
			ar & operand2_;
		}
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_ANDEXP_H__ */

/**
 * @}
 */
