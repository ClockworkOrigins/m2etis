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

/**
 * \addtogroup simulator
 * @ {
 */

#ifndef __M2ETIS_SIM_OMNETKEYPROVIDER_H__
#define __M2ETIS_SIM_OMNETKEYPROVIDER_H__

#include "m2etis/message/key/Key.h"

#include "boost/lexical_cast.hpp"
#include "boost/serialization/array.hpp"

namespace m2etis {
namespace sim {

	class OmNetKeyProvider {
	private:
		unsigned char ip[4];
		unsigned short port;

	public:
		/**
		 * \brief destructor
		 */
		virtual ~OmNetKeyProvider() {}

	protected:
		/**
		 * \brief default constructor
		 */
		OmNetKeyProvider() : port() {
			for (int i = 0; i < 4; ++i) {
				ip[i] = 0;
			}
		}

		/**
		 * \brief transforms key to a string representation
		 */
		std::string keytoStr() const {
			std::ostringstream ret;

			ret << static_cast<int>(ip[0]) << "." << static_cast<int>(ip[1]) << "." << static_cast<int>(ip[2]) << "." << static_cast<int>(ip[3])<< ":" << port;

			return ret.str();
		}

		/**
		 * \brief duplicates a key
		 *
		 * \param[in] key key to be duplicated
		 */
		void setKey(const message::Key<OmNetKeyProvider> & key) {
			ip[0] = key.ip[0];
			ip[1] = key.ip[1];
			ip[2] = key.ip[2];
			ip[3] = key.ip[3];

			port = key.port;
		}

		/**
		 * \brief creates key out of a string
		 *
		 * \param[in] key string representation of a string
		 */
		void setKey(const std::string & key) {
			size_t ipend = key.find(":", 0);

			std::string locip(key.substr(0, ipend));
			size_t pos = 0;
			for (unsigned int i = 0; i < 4; ++i) {
				size_t bytend = key.find(".", pos);
				std::stringstream ss(key.substr(pos, bytend));
				int t;
				ss >> t;
				ip[i] = t;
				pos = bytend + 1;
			}

			port = std::atoi(key.substr(ipend + 1, key.find(":", ipend + 1) - ipend).c_str());
		}

		/**
		 * \brief equals operator of the key class
		 */
		bool equals(const message::Key<OmNetKeyProvider> & rval) const {
			for (unsigned int i = 0; i < 4; ++i) {
				if (ip[i] != rval.ip[i]) {
					return false;
				}
			}
			if (port != rval.port) {
				return false;
			}

			return true;
		}

		/**
		 * \brief lesser than operator for the key
		 */
		bool smaller(const message::Key<OmNetKeyProvider> & rval) const {
			for (unsigned int i = 0; i < 4; ++i) {
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
		void serialize(Archive & ar, unsigned int version) {
			ar & ip;
			ar & port;
		}
	};

} /* namespace sim */
} /* namespace m2etis */

#endif /* __M2ETIS_SIM_OMNETKEYPROVIDER_H__ */

/**
 *  @}
 */
