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

#ifndef __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_PREDICATEIDENTIFIERFACTORY_H__
#define __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_PREDICATEIDENTIFIERFACTORY_H__

#include <set>
#include <vector>

namespace m2etis {
namespace pubsub {
namespace filter {

	class PredicateIdentifierFactory {
	public:
		typedef std::vector<bool>::size_type PredicateID;

		PredicateIdentifierFactory() : max_used_ID_(0) {
			reusableIDs_.insert(0);
		}

		PredicateID createPredicateIdentifier() {
			PredicateID newID;

			if (!reusableIDs_.empty()) {
				newID = *(reusableIDs_.begin());
				reusableIDs_.erase(reusableIDs_.begin());
			} else {
				newID = ++max_used_ID_;
			}

			return newID;
		}

		void freeID(PredicateID id) {
			reusableIDs_.insert(id);
		}

	private:
		PredicateID max_used_ID_;

		// data structure to reuse predicate ids:
		std::set<PredicateID> reusableIDs_;
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_PREDICATEIDENTIFIERFACTORY_H__ */

/**
 *  @}
 */
