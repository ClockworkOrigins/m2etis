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


#ifndef __M2ETIS_UTIL_SINGLETON_H__
#define __M2ETIS_UTIL_SINGLETON_H__

#include "m2etis/util/SystemParameters.h"

#include "boost/interprocess/sync/scoped_lock.hpp"
#include "boost/thread.hpp"

namespace m2etis {
namespace util {

	/**
	* \class Singleton
	* \brief Derive from this templated class to make a class a singleton.
	*		 Refer to the Singleton Design Pattern if it is not clear what a singleton is.
	*		 Declare the ctor private.
	*/
	template<typename T>
	class Singleton {
	public:
		static inline T * GetSingletonPtr() {
			Init();

			return _ptrSingleton;
        }

		static inline T & GetSingleton() {
			Init();

            return *_ptrSingleton;
        }

	protected:
		Singleton() {}
        virtual ~Singleton() {
			boost::unique_lock<boost::mutex> lock(_objMutex);
        	if (_ptrSingleton != NULL) {
        		delete _ptrSingleton;
        		_ptrSingleton = NULL;
        	}
        }
		static T * _ptrSingleton;
		static boost::mutex _objMutex;

	private:
		static void Init() {
			// Double check to avoid overhead locking
			if (!_ptrSingleton) {
				// no need to unlock, because the destructor of boost::unique_lock does this for us
				boost::unique_lock<boost::mutex> lock(_objMutex);
				if (!_ptrSingleton) {
					_ptrSingleton = new T();
				}
			}
		}

		Singleton(const Singleton &) {}
		const Singleton& operator=(const Singleton &) { return *this; }
	};

	template<typename T> T * Singleton<T>::_ptrSingleton = 0;
    template<typename T> boost::mutex  Singleton<T>::_objMutex;

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_SINGLETON_H__ */
