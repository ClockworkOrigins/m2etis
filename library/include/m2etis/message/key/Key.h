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

#ifndef __M2ETIS_MESSAGE_KEY_H__
#define __M2ETIS_MESSAGE_KEY_H__

#include <string>

#include "boost/serialization/base_object.hpp"

namespace m2etis {
namespace message {

	/**
	 * \class Key
	 * \brief Key
	 *
	 * Long Desc
	 *
	 */
    template<class KeyProvider>
	class Key : public KeyProvider {
	public:
		/**
		 * \brief Default constructor
		 */
		Key() : KeyProvider() {
        }

		/**
		 * \brief Copy Constructor
		 * \param[in] rval key to copy
		 */
		Key(const Key & rval) {
            KeyProvider::setKey(rval);
		}

		/**
      	 * \brief Constructor
      	 * \param[in] val key
      	 */
		explicit Key(const std::string & val) : KeyProvider() {
            KeyProvider::setKey(val);
        }

        explicit Key(const std::vector<unsigned char> & val) : KeyProvider() {
            KeyProvider::deserialize(val);
		}

		inline static size_t size() {
			return KeyProvider::KEYLENGTH;
		}

		/**
      	 * \brief Gets a string representation of the key
      	 *
      	 * \return Returns a string representation of the key
      	 */
		inline std::string toStr() const {
			return KeyProvider::keytoStr();
		}

        Key<KeyProvider> & operator=(const Key<KeyProvider> & rval) {
            KeyProvider::setKey(rval);
            return *this;
        }

        bool operator==(const Key<KeyProvider> & rval) const {
            return KeyProvider::equals(rval);
        }

        bool operator!=(const Key<KeyProvider> & rval) const {
            return !(*this == rval);
        }

        bool operator<(const Key<KeyProvider> & rval) const {
        	return KeyProvider::smaller(rval);
        }

        bool operator>(const Key<KeyProvider> & rval) const {
        	return !(*this < rval) && (*this != rval);
        }

    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, unsigned int /* version */) {
        	ar & boost::serialization::base_object<KeyProvider>(*this);
        }
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_KEY_H__ */

/**
 * @}
 */
