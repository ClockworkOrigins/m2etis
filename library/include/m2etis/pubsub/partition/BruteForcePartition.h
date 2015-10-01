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

#ifndef __M2ETIS_PUBSUB_PARTITION_BRUTEFORCEPARTITION_H__
#define __M2ETIS_PUBSUB_PARTITION_BRUTEFORCEPARTITION_H__

#include <map>

#include "m2etis/pubsub/filter/FilterPredicate.h"
#include "m2etis/pubsub/filter/filtervisitors/VariableAssignmentVisitor.h"
#include "m2etis/pubsub/partition/BasePartition.h"

namespace m2etis {
namespace pubsub {
namespace partition {

	template <typename NetworkType, typename EventType>
	class BruteForcePartition : public BasePartition<NetworkType, EventType> {
	public:
		typedef boost::shared_ptr<EventType> PayloadPtr;

		static const bool DYNAMIC_PARTITION = false;

		BruteForcePartition() : partition_filter_vector_(std::vector<boost::shared_ptr<filter::FilterExp<EventType>> >()) {
		}

		explicit BruteForcePartition(const std::vector<boost::shared_ptr<filter::FilterExp<EventType>> > & partition_filter_vector) : partition_filter_vector_(partition_filter_vector) {
		}

		virtual ~BruteForcePartition() {}

		void createRendezvousPartition(const typename NetworkType::Key & rendezvous) {
		}

		// returns vector of all tree indexes of channel
		std::vector<int> getTreeNames() {
			std::vector<int> ret;
			if (partition_filter_vector_.empty()) {
				ret.push_back(0);
				return ret;
			}
			for (std::vector<int>::size_type i = 0; i != partition_filter_vector_.size(); ++i) {
				ret.push_back(i);
			}
			return ret;
		}

		// returns tree on which the given message should be published (matching algorithm)
		std::vector<int>::size_type getPublishTree(const PayloadPtr message, const typename NetworkType::Key & self) {
			if (partition_filter_vector_.empty()) {
				return 0;
			}

			std::vector<int>::size_type i = 0;
			for (auto current_filter : partition_filter_vector_) {
				filter::MatchVisitor<EventType> match_filter_visitor(*message);

				current_filter->Accept(match_filter_visitor);
				if (match_filter_visitor.get_result()) {
					M2ETIS_LOG_INFO("BruteForcePartition", "publishing message on tree number " << i);
					return i;
				}
				++i;
			}
			M2ETIS_THROW_API("BruteForcePartition", "No matching tree for publish message found.");
		}

		// returns trees which have a static filter matching the given dynamic filter (overlap algorithm)
		std::vector<unsigned int> getSubscribeTrees(boost::shared_ptr<filter::FilterExp<EventType> > dynamic_filter) {
			std::vector<unsigned int> overlapping_trees;
			if (partition_filter_vector_.empty()) {
				overlapping_trees.push_back(0);
				return overlapping_trees;
			}
			unsigned int tree_index = 0;

			for (auto partition_filter : partition_filter_vector_) {
				if (isOverlap(partition_filter, dynamic_filter)) {
					overlapping_trees.push_back(tree_index);
				}
				++tree_index;
			}
			return overlapping_trees;
		} // getSubscribeTrees

		boost::shared_ptr<filter::FilterExp<EventType> > getPredicate(unsigned int id) {
			assert(id < partition_filter_vector_.size());
			return partition_filter_vector_[id];
		}

		bool createPartition(const typename NetworkType::Key & root) {
			return false;
		}

		void addPartition(boost::shared_ptr<filter::FilterExp<EventType> > predicate, const typename NetworkType::Key & root) {
		}

		void removePartition(unsigned int id) {
		}

		void changePredicate(unsigned int id, boost::shared_ptr<filter::FilterExp<EventType> > predicates) {
		}

		void changeRoot(unsigned int id, typename NetworkType::Key & root) {
		}

	private:
		bool isOverlap(const boost::shared_ptr<filter::FilterExp<EventType> > static_filter, const boost::shared_ptr<filter::FilterExp<EventType> > dynamic_filter) {
			// step 1: test expression for satisfiability and determine predicate assignment candidates:
			auto intersection_filter = std::make_shared<filter::AndExp<EventType> >(static_filter, dynamic_filter);

			filter::VariableAssignmentVisitor<EventType> variable_assignment_filter_visitor;
			intersection_filter->Accept(variable_assignment_filter_visitor); // determining number of predicates

			unsigned long number_of_assignment_possibilities = static_cast<unsigned long>(pow(2.0, variable_assignment_filter_visitor.get_predicate_number()));
			std::vector<unsigned long> fulfilling_predicate_assignments;

			for (unsigned long predicate_assignment = 0; predicate_assignment != number_of_assignment_possibilities; ++predicate_assignment) {
				variable_assignment_filter_visitor.reset();
				variable_assignment_filter_visitor.set_predicate_assignment(predicate_assignment);
				intersection_filter->Accept(variable_assignment_filter_visitor);
				if (variable_assignment_filter_visitor.get_result()) {
					fulfilling_predicate_assignments.push_back(predicate_assignment);
				}
			}

			// step 2: test all candidate predicate assignments from step 1
			for (auto fulfilling_predicate_assignment_iterator = fulfilling_predicate_assignments.begin(); fulfilling_predicate_assignment_iterator != fulfilling_predicate_assignments.end(); ++fulfilling_predicate_assignment_iterator) {
				bool disjoint_predicates_found = false;
				auto fulfilling_predicate_assignment = *fulfilling_predicate_assignment_iterator;
				// testing every predicate against every other predicate for overlap

				// loop over bits in predicate assignment, check for 1 bits (predicates (indexes), that have to be true):
				for (int current_predicate_number = 0; current_predicate_number != variable_assignment_filter_visitor.get_predicate_number() && !disjoint_predicates_found; ++current_predicate_number) {
					if (bool(fulfilling_predicate_assignment & (1UL << current_predicate_number))) {
						// predicate has to be true in order to fulfill variable assignment, compare with all following predicates
						for (int other_predicate_number = current_predicate_number + 1; other_predicate_number != variable_assignment_filter_visitor.get_predicate_number() && !disjoint_predicates_found; ++other_predicate_number) {
							if (bool(fulfilling_predicate_assignment & (1UL << other_predicate_number))) {
								// predicate has to be true in order to fulfill variable assignment, compare with current predicate
								if (!((variable_assignment_filter_visitor.get_predicate_index())[current_predicate_number])->overlaps((variable_assignment_filter_visitor.get_predicate_index())[other_predicate_number])) {
									disjoint_predicates_found = true; // try next predicate assignment (exit loops for performance reasons in for conditions)
								}
							}
						}
					}
				}
				if (!disjoint_predicates_found) {
					return true; // predicate assignment found, which is a true overlap
				}
			} // loop over predicate assignments, which are possible overlaps

			return false;
		} // function isOverlap

		std::vector<boost::shared_ptr<filter::FilterExp<EventType>> > partition_filter_vector_;
		// one entry for each tree of channel
		// TODO: (Roland) make const including elements
	}; // class BruteForcePartition

} /* namespace partition */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_PARTITION_BRUTEFORCEPARTITION_H__ */
