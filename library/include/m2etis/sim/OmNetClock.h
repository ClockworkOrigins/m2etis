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
 * @ {
 */

#ifndef __M2ETIS_SIM_OMNETCLOCK_H__
#define __M2ETIS_SIM_OMNETCLOCK_H__

#include "boost/function.hpp"

namespace m2etis {
namespace sim {

	class OmNetCallbackInterface;

	class OmNetClock {
	public:
		/**
		 * \brief constructor receiving update method of Clock subclass
		 *
		 * \param[in] f the update method of the Clock subclass, not necessary here
		 */
		explicit OmNetClock(const boost::function<void(void)> & f);

		/**
		 * \brief destructor
		 */
		~OmNetClock();

		/**
		 * \brief returns current time of the simulation
		 *
		 * \param[in] lastTime the time the last update occured
		 */
		long getCurrentTime(long lastTime);

		/**
		 * \brief sets pointer to simulation module
		 *
		 * \param[in] sim pointer to the simulation module
		 */
		void setSim(OmNetCallbackInterface * sim) {
			_sim = sim;
		}

		/**
		 * \brief stops the clock
		 */
		void Stop() {}

	private:
		const long startTime_;
		OmNetCallbackInterface * _sim;

		OmNetClock & operator=(const OmNetClock & o);
		OmNetClock(const OmNetClock & o);
	};

} /* namespace sim */
} /* namespace m2etis */

#endif /* __M2ETIS_SIM_OMNETCLOCK_H__ */

/**
 *  @}
 */
