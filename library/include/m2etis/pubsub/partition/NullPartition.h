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

		void createRendezvousPartition(const typename NetworkType::Key &) {
		}

		std::vector<int> getTreeNames() {
			std::vector<int> ret(1);
			return ret;
		}

		std::vector<int>::size_type getPublishTree(const PayloadPtr, const typename NetworkType::Key &) {
			return ALL;
		}

		std::vector<unsigned int> getSubscribeTrees(boost::shared_ptr<filter::FilterExp<EventType>>) {
			std::vector<unsigned int> overlapping_trees;
			overlapping_trees.push_back(ALL);
			return overlapping_trees;
		}

		boost::shared_ptr<filter::FilterExp<EventType>> getPredicate(unsigned int) {
			return boost::shared_ptr<filter::FilterExp<EventType>>();
		}

		bool createPartition(const typename NetworkType::Key &) {
			return false;
		}

		void addPartition(boost::shared_ptr<filter::FilterExp<EventType>>, const typename NetworkType::Key &) {
		}

		void removePartition(unsigned int) {
		}

		void changePredicate(unsigned int, boost::shared_ptr<filter::FilterExp<EventType>>) {
		}

		void changeRoot(unsigned int, typename NetworkType::Key &) {
		}
	};

} /* namespace partition */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_PARTITION_NULLPARTITION_H__ */

/**
 *  @}
 */
