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
 * \addtogroup message
 * @ {
 */

#ifndef __M2ETIS_MESSAGE_INFO_SECURITYINFO_H__
#define __M2ETIS_MESSAGE_INFO_SECURITYINFO_H__

#include "boost/shared_ptr.hpp"

namespace m2etis {
namespace message {

	struct SecurityInfo {
		typedef boost::shared_ptr<SecurityInfo> Ptr;

		SecurityInfo() {}

		template<typename Archive>
		void serialize(Archive &, const unsigned int) {
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_INFO_SECURITYINFO_H__ */

/**
 *  @}
 */
