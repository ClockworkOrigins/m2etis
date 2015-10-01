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

#ifndef __M2ETIS_MESSAGE_NULLPERSISTENCEINFO_H__
#define __M2ETIS_MESSAGE_NULLPERSISTENCEINFO_H__

#include "m2etis/message/info/PersistenceInfo.h"

#include "boost/serialization/base_object.hpp"

namespace m2etis {
namespace message {

	class NullPersistenceInfo : public PersistenceInfo {
	public:
        typedef boost::shared_ptr<NullPersistenceInfo> Ptr;

        static bool doSerialize(ActionType t) {
        	return false;
        }

    private:
        friend class boost::serialization::access;
		template <typename Archive> void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<PersistenceInfo>(*this);
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_NULLPERSISTENCEINFO_H__ */
