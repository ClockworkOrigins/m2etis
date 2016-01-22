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

#ifndef __M2ETIS_PUBSUB_FILTER_DECISIONTREE_DECISIONTREELEAF_H__
#define __M2ETIS_PUBSUB_FILTER_DECISIONTREE_DECISIONTREELEAF_H__

#include <set>
#include <string>

#include "boost/shared_ptr.hpp"

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class DecisionTreeLeaf : public DecisionTreeNode<EventType, NetworkType> {
	public:
		DecisionTreeLeaf() = default;

		explicit DecisionTreeLeaf(const typename NetworkType::Key & subscriber) {
			addSubscriber(subscriber);
		}

		// v is a leaf node of Tree: output(v)
		virtual void visit(const EventType &, std::set<typename NetworkType::Key> & matching_subscribers) override {
			matching_subscribers.insert(subscribers_.begin(), subscribers_.end());
		}

		// merges another subscription tree into the existing one
		virtual void merge(boost::shared_ptr<DecisionTreeNode<EventType, NetworkType>> &, typename NetworkType::Key subscriber) override {
			changeSubscriber(subscriber);
		} // merge

		virtual void changeSubscriber(typename NetworkType::Key subscriber) override {
			subscribers_.clear();
			subscribers_.insert(subscriber);
		} // changeSubscriber

		// adds subscriber
		virtual void addSubscriber(const typename NetworkType::Key & subscriber) override {
			subscribers_.insert(subscriber);
		}

		// removes the subscriber and returns the number of subscribers left
		virtual void removeSubscriber(const typename NetworkType::Key & subscriber) override {
			subscribers_.erase(subscriber);
		}

		virtual bool hasSubscriber() override {
			if (subscribers_.size() == 0) {
				return false;
			}
			return true;
		}

		// Testing whether there are subscriptions to the subscriber
		virtual bool hasSubscription(typename NetworkType::Key subscriber) override {
			if (subscribers_.find(subscriber) != subscribers_.end()) {
				return true;
			}
			return false;
		}

		// for debugging purposes:
		virtual operator std::string() const {
			return "leaf node with first subscriber: " + (*(subscribers_.begin())).toStr();
		};

		virtual ~DecisionTreeLeaf() {}

	private:
		std::set<typename NetworkType::Key> subscribers_;

		friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<DecisionTreeNode<EventType, NetworkType> >(*this);
			ar & subscribers_;
		}
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_DECISIONTREE_DECISIONTREELEAF_H__ */

/**
 *  @}
 */
