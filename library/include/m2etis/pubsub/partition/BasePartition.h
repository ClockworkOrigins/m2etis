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

#ifndef __M2ETIS_PUBSUB_PARTITION_BASEPARTITION_H__
#define __M2ETIS_PUBSUB_PARTITION_BASEPARTITION_H__

#include "boost/assign/list_of.hpp"

namespace m2etis {
namespace pubsub {
namespace partition {

	template <typename NetworkType, typename EventType>
	class BasePartition {
	public:
		typedef boost::shared_ptr<EventType> PayloadPtr;

		BasePartition() {
		}

		virtual ~BasePartition() {}

		/**
		 * \brief creates a base partition for the RP (dynamic)
		 */
		virtual void createRendezvousPartition(const typename NetworkType::Key & rendezvous) = 0;

		/**
		 * \brief returns amount of partitions this strategy contains (static & dynamic)
		 */
		virtual std::vector<int> getTreeNames() = 0;

		/**
		 * \brief returns the tree index the given node with the given payload should publish on (static & dynamic)
		 */
		virtual std::vector<int>::size_type getPublishTree(const PayloadPtr message, const typename NetworkType::Key & self) = 0;

		/**
		 * \brief returns the tree indices of all trees matching the predicate to subscribe on (static & dynamic)
		 */
		virtual std::vector<unsigned int> getSubscribeTrees(boost::shared_ptr<filter::FilterExp<EventType>> dynamic_filter) = 0;

		/**
		 * \brief returns the predicate for the given partition id (dynamic)
		 */
		virtual boost::shared_ptr<filter::FilterExp<EventType>> getPredicate(size_t id) = 0;

		/**
		 * \brief can create a new partition for this channel if a new node (root) joins, return true, if a new partition was created, otherwise false (dynamic)
		 */
		virtual bool createPartition(const typename NetworkType::Key & root) = 0;

		/**
		 * \brief adds a new partition to the strategy at nodes not being RP (dynamic)
		 */
		virtual void addPartition(boost::shared_ptr<filter::FilterExp<EventType> > predicate, const typename NetworkType::Key & root) = 0;

		/**
		 * \brief removes the partition with the given id (dynamic)
		 */
		virtual void removePartition(size_t id) = 0;

		/**
		 * \brief changes the filter predicate for the partition with the given id (dynamic)
		 */
		virtual void changePredicate(size_t id, boost::shared_ptr<filter::FilterExp<EventType>> predicates) = 0;
		
		virtual void changeRoot(size_t id, typename NetworkType::Key & root) = 0;

		virtual std::string toString() {
			return "BasePartition";
		}
	};

} /* namespace partition */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_PARTITION_BASEPARTITION_H__ */

/**
 *  @}
 */
