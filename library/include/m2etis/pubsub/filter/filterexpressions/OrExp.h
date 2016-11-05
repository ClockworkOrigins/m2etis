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

#ifndef __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_OREXP_H__
#define __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_OREXP_H__

#include <functional>

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType>
	class OrExp : public FilterExp<EventType> {
	public:
		typedef EventType schema; // needed for operator overloading
		OrExp() {} // for boost serialization
		OrExp(const boost::shared_ptr<FilterExp<EventType>> op1, const boost::shared_ptr<FilterExp<EventType>> op2) : operand1_(op1), operand2_(op2) {}
		virtual ~OrExp() {}

		virtual void Accept(FilterVisitor<EventType> & filter_visitor) const override {
			operand1_->Accept(filter_visitor);
			operand2_->Accept(filter_visitor);
			filter_visitor.Visit(this);
		}

		virtual operator boost::shared_ptr<FilterExp<EventType>>() const {
			M2ETIS_LOG_DEBUG("OrExp", "conversion operator in FilterExp to shared_ptr<FilterExp<EventType>>. Originally only intented for debugging purposes");
			return boost::make_shared<OrExp<EventType> >(*this);
		}

		// for debugging purposes:
		virtual operator std::string() const override {
			return (std::string("OrExp") + "(\n" + std::string(*operand1_) + ", \t\t  " + std::string(*operand2_) + ")");
		}

	private:
		boost::shared_ptr<FilterExp<EventType>> operand1_;
		boost::shared_ptr<FilterExp<EventType>> operand2_;

		virtual bool doCompare(const FilterExp<EventType> & other_filter) const override {
			const OrExp * other_OrExp = dynamic_cast<const OrExp *>(&other_filter);

			if (other_OrExp) {
				return (*operand1_ == *(other_OrExp->operand1_) && *operand2_ == *(other_OrExp->operand2_));
			} else {
				return false;
			}
		}

		virtual size_t doHash() const override {
			return std::hash<FilterExp<EventType>>()(*operand1_) ^ std::hash<FilterExp<EventType>>()(*operand2_);
		}

		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<FilterExp<EventType>>(*this);
			ar & operand1_;
			ar & operand2_;
		}
	}; // OrExp

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTEREXPRESSIONS_OREXP_H__ */

/**
 * @}
 */
