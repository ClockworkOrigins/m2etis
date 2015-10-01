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

#ifndef __M2ETIS_PUBSUB_PARTITION_NULLPARTITION_H__
#define __M2ETIS_PUBSUB_PARTITION_NULLPARTITION_H__

#include "m2etis/pubsub/partition/BasePartition.h"

#include "boost/assign/list_of.hpp"

namespace m2etis {
namespace pubsub {
namespace partition {

	template <typename NetworkType, typename EventType>
	class NullPartition : public BasePartition<NetworkType, EventType> {
	public:
		typedef boost::shared_ptr<EventType> PayloadPtr;

		static const bool DYNAMIC_PARTITION = false;

		enum TreeName {
			ALL
		};

		NullPartition() {
		}

		virtual ~NullPartition() {}

		void createRendezvousPartition(const typename NetworkType::Key & rendezvous) {
		}

		std::vector<int> getTreeNames() {
			std::vector<int> ret(1);
			return ret;
		}

		std::vector<int>::size_type getPublishTree(const PayloadPtr message, const typename NetworkType::Key & self) {
			return ALL;
		}

		std::vector<unsigned int> getSubscribeTrees(boost::shared_ptr<filter::FilterExp<EventType> > dynamic_filter) {
			std::vector<unsigned int> overlapping_trees;
			overlapping_trees.push_back(ALL);
			return overlapping_trees;
		}

		boost::shared_ptr<filter::FilterExp<EventType> > getPredicate(unsigned int id) {
			return boost::shared_ptr<filter::FilterExp<EventType> >();
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
	};

} /* namespace partition */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_PARTITION_NULLPARTITION_H__ */
