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

#ifndef __M2ETIS_PUBSUB_PARTITION_DIRECTBROADCASTPARTITION_H__
#define __M2ETIS_PUBSUB_PARTITION_DIRECTBROADCASTPARTITION_H__

#include <map>
#include <sstream>

#include "m2etis/pubsub/filter/FilterPredicate.h"
#include "m2etis/pubsub/filter/filtervisitors/VariableAssignmentVisitor.h"
#include "m2etis/pubsub/partition/BasePartition.h"

namespace m2etis {
namespace pubsub {
namespace partition {

	template<typename NetworkType, typename EventType>
	class DirectBroadcastPartition : public BasePartition<NetworkType, EventType> {
	public:
		typedef boost::shared_ptr<EventType> PayloadPtr;

		static const bool DYNAMIC_PARTITION = true;

		DirectBroadcastPartition() : partition_filter_vector_() {
		}

		virtual ~DirectBroadcastPartition() {}

		void createRendezvousPartition(const typename NetworkType::Key & rendezvous) {
			partition_filter_vector_.push_back(std::make_pair(boost::shared_ptr<filter::FilterExp<EventType>>(), rendezvous));
		}

		std::vector<int> getTreeNames() {
			std::vector<int> ret;
			if (partition_filter_vector_.empty()) {
				ret.push_back(0);
				return ret;
			}
			for (std::vector<int>::size_type i = 0; i < partition_filter_vector_.size(); ++i) {
				ret.push_back(int(i));
			}
			return ret;
		}

		std::vector<int>::size_type getPublishTree(const PayloadPtr message, const typename NetworkType::Key & self) {
			std::vector<int>::size_type i = 0;
			for (std::pair<boost::shared_ptr<filter::FilterExp<EventType>>, typename NetworkType::Key> current_filter : partition_filter_vector_) {
				if (self == current_filter.second) {
					return i;
				}
				++i;
			}
			return i;
		}

		std::vector<unsigned int> getSubscribeTrees(boost::shared_ptr<filter::FilterExp<EventType> > dynamic_filter) {
			std::vector<unsigned int> overlapping_trees;
			if (partition_filter_vector_.empty()) {
				overlapping_trees.push_back(0);
				return overlapping_trees;
			}
			unsigned int tree_index = 0;

			for (std::pair<boost::shared_ptr<filter::FilterExp<EventType>>, typename NetworkType::Key> partition_filter : partition_filter_vector_) {
				overlapping_trees.push_back(tree_index);
				++tree_index;
			}
			return overlapping_trees;
		}

		boost::shared_ptr<filter::FilterExp<EventType>> getPredicate(size_t id) {
			assert(id < partition_filter_vector_.size());
			return partition_filter_vector_[id].first;
		}

		bool createPartition(const typename NetworkType::Key & root) {
			partition_filter_vector_.push_back(std::make_pair(boost::shared_ptr<filter::FilterExp<EventType>>(), root));
			return true;
		}

		void addPartition(boost::shared_ptr<filter::FilterExp<EventType>> predicate, const typename NetworkType::Key & root) {
			partition_filter_vector_.push_back(std::make_pair(predicate, root));
		}

		void removePartition(size_t id) {
			partition_filter_vector_.erase(partition_filter_vector_.begin() + id);
		}

		void changePredicate(size_t, boost::shared_ptr<filter::FilterExp<EventType>>) {
		}
		
		void changeRoot(size_t id, typename NetworkType::Key & root) {
			partition_filter_vector_[id].second = root;
		}

		std::string toString() {
			std::stringstream ss;
			ss << "DBParition with " << partition_filter_vector_.size() << " elements: (";
			for (unsigned int i = 0; i < partition_filter_vector_.size(); i++) {
				ss << "(" << partition_filter_vector_[i].second.toStr() << ", " << partition_filter_vector_[i].first->toString() << "), ";
			}
			ss << ")";
			return ss.str();
		}
		
	private:
		std::vector<std::pair<boost::shared_ptr<filter::FilterExp<EventType>>, typename NetworkType::Key> > partition_filter_vector_;
	};

} /* namespace partition */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_PARTITION_DIRECTBROADCASTPARTITION_H__ */

/**
 *  @}
 */
