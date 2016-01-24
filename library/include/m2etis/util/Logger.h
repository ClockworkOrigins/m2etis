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
 * \addtogroup util
 * @{
 */

#ifndef __M2ETIS_UTIL_LOGGER_H__
#define __M2ETIS_UTIL_LOGGER_H__


#ifdef WITH_LOGGING
	#define GLOG_NO_ABBREVIATED_SEVERITIES
	#include <glog/logging.h>
#endif


#ifdef WITH_LOGGING
	#define M2ETIS_LOG_DEBUG(module,  message) {\
		 DLOG(INFO) << message;\
	}
	#define M2ETIS_LOG_INFO(module,  message) {\
		LOG(INFO) << message;\
	}
	#define M2ETIS_LOG_WARN(module,  message) {\
		LOG(WARNING) << message;\
	}
	#define M2ETIS_LOG_ERROR(module,  message) {\
		LOG(ERROR) << message;\
	}
	#define M2ETIS_LOG_FATAL(module,  message) {\
		LOG(FATAL) << message;\
	}

	#define M2ETIS_LOG_SETLEVEL(level) {\
		m2etis::util::Logger::GetSingleton().SetLogLevel(level);\
	}
#else
	#define M2ETIS_LOG_DEBUG(module, message) {\
	}
	#define M2ETIS_LOG_INFO(module, message) {\
	}
	#define M2ETIS_LOG_WARN(module, message) {\
	}
	#define M2ETIS_LOG_ERROR(module, message) {\
	}
	#define M2ETIS_LOG_FATAL(module, message) {\
	}

	#define M2ETIS_LOG_SETLEVEL(level) {\
	}
#endif /* WITH_LOGGING */

namespace m2etis {
namespace util {
namespace log {
        /**
		 * \brief Defines values to specify severity/importance of log message.
         *
		 */
#ifdef WITH_LOGGING
	extern bool loggingOn;

	enum LogLevel {
		LOG_INFO = google::GLOG_INFO,
		LOG_WARN = google::GLOG_WARNING,
		LOG_ERROR = google::GLOG_ERROR,
		LOG_FATAL = google::GLOG_FATAL,
		LOG_NONE = 5
	};
#else /* WITH_LOGGING */
	enum LogLevel {
		LOG_INFO = 0,
		LOG_WARN,
		LOG_ERROR,
		LOG_FATAL,
		LOG_NONE
	};
#endif /* WITH_LOGGING */

	void initializeLogging();
	void shutdownLogging();

} /* namespace log */
} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_LOGGER_H__ */

/**
 * @}
 */
