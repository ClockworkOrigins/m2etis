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

#ifndef __M2ETIS_MESSAGE_MESSAGETYPE_H__
#define __M2ETIS_MESSAGE_MESSAGETYPE_H__

#include <cstdint>

namespace m2etis {
namespace message {

    // Mask to calculate the message Number
    static const uint32_t MSG_TYPE_MASK = 65535; // bit 0-15
    static const uint32_t ACTION_TYPE_MASK = 4294901760; // bit 16-31

    // Defining type for messages on a certain channel. May change on embedded architectures. Therefore defined here as typedef
    typedef uint32_t MessageType;

	enum ActionType : uint32_t {
		NONE = 1<<16, // 65536
		SUBSCRIBE = 1<<17, // 131072
		UNSUBSCRIBE = 1<<18, // 262144
		PUBLISH = 1<<19, // 524288
		CONTROL = 1<<20, // 1048576
		NOTIFY = 1<<21, // 2097152
		JOIN = 1<<22, // 4194304
		STATE = 1<<23, // 8388608
		LEAVE = 1<<24, // 16777216
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_MESSAGETYPE_H__ */

/**
 *  @}
 */
