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

#ifndef __M2ETIS_PUBSUB_FILTER_MATCHVISITOR_H__
#define __M2ETIS_PUBSUB_FILTER_MATCHVISITOR_H__

#include <stack>

#include "m2etis/pubsub/filter/FilterPredicate.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template <typename EventType>
	class MatchVisitor : public FilterVisitor<EventType> {
	private:
		MatchVisitor() {}

	public:
		explicit MatchVisitor(const EventType & event) : event_(event) {}

		virtual void Visit(const FilterExp<EventType> *) override {}

		virtual void Visit(const AndExp<EventType> *) override {
			bool operand1 = operand_stack_.top();
			operand_stack_.pop();
			bool operand2 = operand_stack_.top();
			operand_stack_.pop();
			operand_stack_.push(operand1 && operand2);
		}

		virtual void Visit(const OrExp<EventType> *) override {
			// operand_stack_.push(operand_stack_.pop() || operand_stack_.pop());
			bool operand1 = operand_stack_.top();
			operand_stack_.pop();
			bool operand2 = operand_stack_.top();
			operand_stack_.pop();
			operand_stack_.push(operand1 || operand2);
		}

		virtual void Visit(const Predicate<EventType> * predicate) override {
			operand_stack_.push(predicate->match(event_));
		}


		bool get_result() const {
			return operand_stack_.empty() ? 0 : operand_stack_.top();
		}

		void set_event(const EventType & event) { event_ = event; }

	private:
		std::stack<bool> operand_stack_;  // stack to store operands (=results of 2 children) of match
		EventType event_; // event to be matched against
}; // class MatchVisitor

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_MATCHVISITOR_H__ */

/**
 *  @}
 */
