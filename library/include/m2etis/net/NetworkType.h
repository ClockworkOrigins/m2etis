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
 * \addtogroup net
 * @{
 */

#ifndef __M2ETIS_NET_NETWORKTYPE_H__
#define __M2ETIS_NET_NETWORKTYPE_H__

#include "m2etis/util/Exceptions.h"

#include "m2etis/message/MessageType.h"
#include "m2etis/net/NetworkName.h"

namespace m2etis {

namespace message {
	template<class T>
	class Key;

	class SHA1KeyProvider;
	class IPv4KeyProvider;

	template<class S>
	class NetworkMessage;
} /* namespace message */

namespace net {

	class NullProvider {
	public:
		NullProvider() {
			M2ETIS_THROW_FAILURE("NullProvider", "This class should never be instantiated", 1);
		}
	};

	template<class NetType>
	class KeyPChooser {
	public:
		typedef NullProvider KeyProvider;
	};

	template<NetworkName nettype>
	class NetworkType {
	public:
		enum {
			value = nettype
		};

		typedef NetworkName value_type;
		typedef NetworkType<nettype> type;
		typedef typename KeyPChooser<NetworkType<nettype>>::KeyProvider KeyProvider;
		typedef message::Key<KeyProvider> Key;
		typedef message::NetworkMessage<type> Message;
		typedef message::MessageType MessageType;
	};

	template<>
	class KeyPChooser<NetworkType<TCP>> {
	public:
		typedef message::IPv4KeyProvider KeyProvider;
	};

	template<>
	class KeyPChooser<NetworkType<UDP>> {
	public:
		typedef message::IPv4KeyProvider KeyProvider;
	};

	template<>
	class KeyPChooser<NetworkType<clockTCP>> {
	public:
		typedef message::IPv4KeyProvider KeyProvider;
	};

} /* namespace net */
} /* namespace m2etis */

#endif /* __M2ETIS_NET_NETWORKTYPE_H__ */

/**
 * @}
 */
