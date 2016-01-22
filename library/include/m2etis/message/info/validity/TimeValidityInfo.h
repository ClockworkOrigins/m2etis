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
 * @ {
 */

#ifndef __M2ETIS_MESSAGE_TIMEVALIDITYINFO_H__
#define __M2ETIS_MESSAGE_TIMEVALIDITYINFO_H__

#include "m2etis/message/info/ValidityInfo.h"

#include "boost/serialization/base_object.hpp"

namespace m2etis {
namespace message {

	class TimeValidityInfo : public ValidityInfo {
	public:
        typedef boost::shared_ptr<TimeValidityInfo> Ptr;

        static bool doSerialize(ActionType t) {
        	if (t == PUBLISH || t == NOTIFY) {
        		return true;
        	}

        	return false;
        }

        uint64_t _timeStamp = UINT64_MAX;

    private:
        friend class boost::serialization::access;

		template<typename Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<ValidityInfo>(*this);
			ar & _timeStamp;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_TIMEVALIDITYINFO_H__ */

/**
 *  @}
 */
