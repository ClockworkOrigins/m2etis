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

#ifndef __M2ETIS_MESSAGE_IPV4KEYPROVIDER_H__
#define __M2ETIS_MESSAGE_IPV4KEYPROVIDER_H__

#include <cstring>

#include "m2etis/message/key/Key.h"

namespace m2etis {
namespace message {

	class IPv4KeyProvider {
    protected:
		uint8_t ip[4];
		uint16_t port;

	public:
		std::string ipStr() const {
			std::ostringstream ret;
			ret << int(ip[0]) << "." << int(ip[1]) << "." << int(ip[2]) << "." << int(ip[3]);
			return ret.str();
		}

		inline std::string portStr() {
			return std::to_string(port);
		}

		inline uint16_t getPort() const {
			return port;
		}

		virtual ~IPv4KeyProvider() {}

	protected:
		IPv4KeyProvider() : port(0) {
			for (int i = 0; i < 4; ++i) {
				ip[i] = 0;
			}
		}

		std::string keytoStr() const {
			std::ostringstream ret;
			ret << int(ip[0]) << "." << int(ip[1]) << "." << int(ip[2]) << "." << int(ip[3])<< ":" << port;
			return ret.str();
		}

		void setKey(const Key<IPv4KeyProvider> & key) {
			memcpy(&ip, &(key.ip), sizeof(ip));
			port = key.port;
		}

		void setKey(const std::string & key) {
			size_t ipend = key.find(":", 0);
			std::string locip(key.substr(0, ipend));
			size_t pos = 0;
			for (size_t i = 0; i < 4; ++i) {
				size_t bytend = key.find(".", pos);
				std::stringstream ss(key.substr(pos, bytend));
				int t;
				ss >> t;
				ip[i] = uint8_t(t);
				pos = bytend + 1;
			}
			std::stringstream sport(key.substr(ipend + 1, key.length() - ipend - 1));
			sport >> port;
		}

		bool equals(const Key<IPv4KeyProvider> & rval) const {
			for (size_t i = 0; i < 4; ++i) {
				if (ip[i] != rval.ip[i]) {
					return false;
				}
			}
			if (port != rval.port) {
				return false;
			}

			return true;
		}

		bool smaller(const Key<IPv4KeyProvider> & rval) const {
			for (size_t i = 0; i < 4; ++i) {
				if (ip[i] != rval.ip[i]) {
					if (ip[i] < rval.ip[i]) {
						return true;
					} else {
						return false;
					}
				}
			}
			if (port != rval.port) {
				if (port < rval.port) {
					return true;
				} else {
					return false;
				}
			}

			return false;
		}

	private:
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, unsigned int /*version*/) {
			ar & ip;
			ar & port;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_IPV4KEYPROVIDER_H__ */

/**
 *  @}
 */
