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

#include "m2etis/sim/OmNetClock.h"

#include "m2etis/sim/OmNetCallbackInterface.h"

namespace m2etis {
namespace sim {

	OmNetClock::OmNetClock(const boost::function<void(void)> & f) : startTime_(0L), _sim() {
	}

	OmNetClock::~OmNetClock() {
	}

	long OmNetClock::getCurrentTime(long oldTime) {
		return _sim->getSimTime();
	}

} /* namespace sim */
} /* namespace m2etis */
