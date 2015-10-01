/**
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


#ifndef __M2ETIS_PUBSUB_FILTER_VARIABLEASSIGNMENTVISITOR_H__
#define __M2ETIS_PUBSUB_FILTER_VARIABLEASSIGNMENTVISITOR_H__

#include <stack>

#include "m2etis/pubsub/filter/FilterPredicate.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template <typename EventType>
	class VariableAssignmentVisitor : public FilterVisitor<EventType> {
	public:
		VariableAssignmentVisitor() : predicate_assignment_(0), predicate_number_(0) {} // determining the size of the tree
		explicit VariableAssignmentVisitor(unsigned long predicate_assignment) : predicate_assignment_(predicate_assignment), predicate_number_(0) {} // evaluating one variable assignment

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

		virtual void Visit(const Predicate<EventType> * current_predicate) override {
			operand_stack_.push(static_cast<bool>(predicate_assignment_ & (1UL << predicate_number_)));
			predicate_index_[predicate_number_] = current_predicate;
			++predicate_number_;
		}

		void reset() {
			if (!operand_stack_.empty()) {
				operand_stack_.pop();
			}
			predicate_number_ = 0;
		}

		bool get_result() const {
			return operand_stack_.empty() ? 0 : operand_stack_.top();
		}

		int get_predicate_number() const {
			return predicate_number_;
		}

		std::map<int, const Predicate<EventType> *> get_predicate_index() const {
			return predicate_index_;
		}

		void set_predicate_assignment(unsigned long predicate_assignment) {
			predicate_assignment_ = predicate_assignment;
		}

	private:
		std::stack<bool> operand_stack_;  // stack to store operands (=results of 2 children) of match
		unsigned long predicate_assignment_;
		// holds the current assignemt of values to variables
		// first predicate in FilterExp tree is first bit in predicate_assignment, second predicate = second bit ...
		// sizeof(unsigned long)*8 predicates at most
		int predicate_number_; // after traversal equal to number of predicates in tree
		std::map<int,  const Predicate<EventType> *> predicate_index_; // to determine predicate at position in predicate_assignment_
}; // class VariableAssignmentVisitor

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_VARIABLEASSIGNMENTVISITOR_H__ */
