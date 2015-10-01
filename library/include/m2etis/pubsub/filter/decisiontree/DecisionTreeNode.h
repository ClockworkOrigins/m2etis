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

#ifndef __M2ETIS_PUBSUB_FILTER_DECISIONTREE_DECISIONTREENODE_H__
#define __M2ETIS_PUBSUB_FILTER_DECISIONTREE_DECISIONTREENODE_H__

#include <set>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class DecisionTreeNode {
	public:
		virtual ~DecisionTreeNode() {
		}

		virtual void visit(const EventType & event, std::set<typename NetworkType::Key> &) = 0;

		// merges with another decision tree. needed for filter propagation
		virtual void merge(boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > & other_tree, typename NetworkType::Key subscriber) = 0;

		virtual void changeSubscriber(typename NetworkType::Key subscriber) = 0;

		// adds subscriber
		virtual void addSubscriber(const typename NetworkType::Key & subscriber) {
			M2ETIS_THROW_API("DecisionTreeNode", "addSubscriber called for non-leaf node.");
		}

		// Removes the node from the tree;
		virtual void removeSubscriber(const typename NetworkType::Key & subscriber) {
			M2ETIS_THROW_API("DecisionTreeNode", "removeSubscriber called for non-leaf node.");
		}

		virtual bool hasSubscriber() {
			M2ETIS_THROW_API("DecisionTreeNode", "hasSubscriber called for non-leaf node.");
		}

		virtual bool hasSubscription(typename NetworkType::Key subscriber) = 0;

		// for debugging purposes:
		virtual operator std::string() const {
			return typeid(*this).name();
		}

		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int version) {
		}
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_DECISIONTREE_DECISIONTREENODE_H__ */
