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

#include "m2etis/util/Logger.h"

namespace m2etis {
namespace util {
namespace log {

#ifdef WITH_LOGGING
	bool loggingOn = false;
#endif

	void initializeLogging() {
#ifdef WITH_LOGGING
		if (!loggingOn) {
			FLAGS_alsologtostderr = 1;
			// FLAGS_log_dir = "log"; // This fails if subdirectory "log" doesn't exist
			google::InitGoogleLogging("m2etis");
			LOG(INFO) << "m2etis logging initialized.";
			loggingOn = true;
		}
#endif /* WITH_LOGGING */
	}

	void shutdownLogging() {
#ifdef WITH_LOGGING
		if (loggingOn) {
			LOG(INFO) << "m2etis logging shutdown.";
			google::ShutdownGoogleLogging();
			loggingOn = false;
		}
#endif /* WITH_LOGGING */
	}

} /* namespace log */
} /* namespace util */
} /* namespace m2etis */
