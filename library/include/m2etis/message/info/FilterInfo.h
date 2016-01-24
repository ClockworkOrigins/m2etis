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
 * \addtogroup message
 * @{
 */

// necessary filter type exports. Each new constraint type (filter attribute) has to be addded here.

#ifndef __M2ETIS_MESSAGE_INFO_FILTERINFO_H__
#define __M2ETIS_MESSAGE_INFO_FILTERINFO_H__

#include "m2etis/pubsub/filter/FilterPredicate.h"

#include "boost/shared_ptr.hpp"

namespace m2etis {
namespace message {

	struct FilterInfo {
    	typedef boost::shared_ptr<FilterInfo> Ptr;

    	// TODO: (Daniel) really necessary for every strategy (e.g. NullFilter)???
    	bool isUnsubscribe_ = false; 	// unsubscripitons of single filters are sent via subscribe
										// messages as well to avoid corrupting routing strategies.
										// however, the filter strategies may store here whether the subscribe message actually contains a filter
										// to be removed

		FilterInfo() {}

		virtual ~FilterInfo() {}

		friend class boost::serialization::access; // not sure if necessary
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & isUnsubscribe_;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_INFO_FILTERINFO_H__ */

/**
 *  @}
 */
