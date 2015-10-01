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

#include "m2etis/util/Exceptions.h"
#include "m2etis/util/ExceptionQueue.h"

namespace m2etis {
namespace util {

	m2exception::m2exception(const util::log::LogLevel loglevel, const std::string & logmodule, const std::string & logmsg, const std::string & file, int line) {
		(*this) << excinfo_log(loginfo(loglevel, logmsg, logmodule, file, line));
	}

	m2exception::m2exception(const util::log::LogLevel loglevel, const std::string & logmodule, const std::string & logmsg, const std::string & file, int line, int errorcode) {
		(*this) << excinfo_log(loginfo(loglevel, logmsg, logmodule, file, line, errorcode));
	}

	void m2exception::writeLog() {
#ifdef WITH_LOGGING
		if (loginfo const * info = boost::get_error_info<excinfo_log>(*this)) {
			LOG(WARNING) << info->module << ": " << info->message;
		}
#endif
	}

	int m2exception::getErrorcode() {
		if (loginfo const * info = boost::get_error_info<excinfo_log>(*this)) {
			return info->errorcode;
		} else {
			return -1;
		}
	}

	const char * m2exception::what() const throw() {
		if (loginfo const * info = boost::get_error_info<excinfo_log>(*this)) {
			std::ostringstream ss;
			ss << "(" << info->file << ":" << info->line << ")  " << info->module << ": " << info->message;
			return ss.str().c_str();
		}
		return "No Message!";
	}

	void m2exception::PassToMain() {
		if (loginfo const * info = boost::get_error_info<excinfo_log>(*this)) {
			ExceptionQueue::enqueue(*info);
		} else {
			M2ETIS_LOG_FATAL("Exceptions", "Trying to enqueue SubsystemException without loginfo attached.")
		}
	}

	SystemFailureException::SystemFailureException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const int line, const int errorcode) : m2exception(util::log::LOG_FATAL, logmodule, logmsg, file, line, errorcode) {
	}

	ApiException::ApiException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const int line) : m2exception(util::log::LOG_FATAL, logmodule, logmsg, file, line) {
	}

} /* namespace util */
} /* namespace m2etis */
