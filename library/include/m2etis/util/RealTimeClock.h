/*
 * Copyright  2011-2012  Prof. Dr. Richard Lenz, Thomas Fischer
 *
 * Licensed  under  the  Apache  License,  Version  2.0  (the  "License");
 * you  may  not  use  this  file  except  in  compliance  with  the  License.
 * You  may  obtain  a  copy  of  the  License  at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless  required  by  applicable  law  or  agreed  to  in  writing,  software
 * distributed  under  the  License  is  distributed  on  an  "AS IS"  BASIS,
 * WITHOUT  WARRANTIES  OR  CONDITIONS  OF  ANY  KIND,  either  express  or  implied.
 * See  the  License  for  the  specific  language  governing  permissions  and
 * limitations  under  the  License.
 *
 */

#ifndef __M2ETIS_UTIL_REALTIMECLOCK_H__
#define __M2ETIS_UTIL_REALTIMECLOCK_H__

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/function.hpp"
#include "boost/thread.hpp"

namespace m2etis {
namespace util {

	class RealTimeClock {
	public:
		explicit RealTimeClock(const boost::function<void(void)> & f);
		virtual ~RealTimeClock();

		uint64_t getCurrentTime(uint64_t lastTime);

		void Stop();

	private:
		RealTimeClock & operator=(const RealTimeClock & o) = delete;
		RealTimeClock(const RealTimeClock & o) = delete;

		void clockUpdater();

		const boost::posix_time::ptime startTime_;

		boost::function<void(void)> update_;

		boost::thread thread_;

		volatile bool _running;
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_REALTIMECLOCK_H__ */
