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
 * \addtogroup simulator
 * @{
 */

#ifndef __M2ETIS_UTILS_RANDOM_H__
#define __M2ETIS_UTILS_RANDOM_H__

#include <cmath>
#include <random>

#include "m2etis/util/Exceptions.h"

#include "boost/date_time.hpp"
#include "boost/thread/mutex.hpp"

namespace m2etis {
namespace util {

	/**
	 * \brief creates random numbers
	 */
	class Random {
	public:
		/**
		 * \brief returns a random number in range of unsigned int
		 */
		static unsigned int rand() {
			boost::mutex::scoped_lock l(lock);
			return random.getRand();
		}

		/**
		 * \brief returns a random number in the range between 0 and max
		 */
		static unsigned int rand(unsigned int max) {
			if (max == 0) {
				M2ETIS_THROW_API("i6eRandom", "max value has to be grater than zero")
			}
			return rand() % max;
		}

		/**
		 * \brief returns a random number in the range between min and max
		 */
		static unsigned int rand(unsigned int min, unsigned int max) {
			if (min >= max) {
				M2ETIS_THROW_API("i6eRandom", "max value has to be greater than min value")
			}
			return (rand() % (max - min)) + min;
		}

	private:
		/**
		 * \brief
		 */
		static Random random;

		/**
		 * \brief
		 */
		std::minstd_rand * _linear;

		/**
		 * \brief
		 */
		static boost::mutex lock;

		/**
		 * \brief default constructor
		 */
		Random() : _linear(new std::minstd_rand(17)) {
		}

		/**
		 * \brief destructor
		 */
		~Random() {
			delete _linear;
		}

		/**
		 * \brief returns the random number generated by intern generator
		 */
		unsigned int getRand() { return (*_linear)(); }
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTILS_RANDOM_H__ */

/**
 * @}
 */
