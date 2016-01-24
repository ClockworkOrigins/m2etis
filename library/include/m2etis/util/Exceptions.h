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

#ifndef __M2ETIS_UTIL_EXCEPTIONS_H__
#define __M2ETIS_UTIL_EXCEPTIONS_H__

#include "m2etis/util/Logger.h"
#include "m2etis/util/SystemParameters.h"

#include "boost/exception/all.hpp"

/**
 * \brief throws on internal errors
 */
#define M2ETIS_THROW_FAILURE(module, message, errorcode) {\
		throw m2etis::util::SystemFailureException(module, message, __FILE__, __LINE__, errorcode);\
	}

/**
 * \brief throws on wrong API usage
 */
#define M2ETIS_THROW_API(module, message) {\
		throw m2etis::util::ApiException(module, message, __FILE__, __LINE__);\
	}

namespace m2etis {
namespace util {

	/**
	 * \brief Internal data structure for exceptions.
	 */
	struct M2ETIS_API loginfo {
        util::log::LogLevel level;
		std::string message;
		std::string module;
		std::string file;
		int line;
		int errorcode;

	public:
		loginfo(const log::LogLevel l, const std::string msg, const std::string m, const std::string f, int li)
			: level(l), message(msg), module(m), file(f), line(li), errorcode(-1) {
		}

		loginfo(const log::LogLevel l, const std::string msg, const std::string m, const std::string f, int li, int errcode)
					: level(l), message(msg), module(m), file(f), line(li), errorcode(errcode) {
		}
	};

	typedef boost::error_info<struct tag_logmessage, loginfo> excinfo_log;

	/**
	 * \class i6exception
	 * \brief Base type for custom exceptions.
	 *
	 * Base type for custom exceptions.
	 */
	struct m2exception : virtual boost::exception, virtual std::exception {
	public:
		/**
		 * \brief Writes the exception's logmessage.
		 */
		M2ETIS_API void writeLog();

		/**
		 * \brief Returns the exception's errorcode or -1 if no errorcode is present.
		 */
		M2ETIS_API int getErrorcode();

		/**
		 * \brief Overrides std::exception method to output message to console.
		 */
		M2ETIS_API virtual const char * what() const throw();

		/**
		 * \brief writes the error message to the ExceptionQueue
		 */
		M2ETIS_API void PassToMain();

	protected:
		M2ETIS_API m2exception(const util::log::LogLevel loglevel, const std::string & logmodule, const std::string & logmsg, const std::string & file, int line);

		M2ETIS_API m2exception(const util::log::LogLevel loglevel, const std::string & logmodule, const std::string & logmsg, const std::string &  file, int line, int errorcode);
	};


	/**
	 * \class SystemFailureException
	 * \brief Critical exception that cannot be repaired by a single subsystem.
	 *
	 * Critical exception that cannot be repaired by a single subsystem. Most likely fatal.
	 */
	struct SystemFailureException : virtual m2exception {
	public:
		M2ETIS_API SystemFailureException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const int line, const int errorcode);
	};

	/**
	 * \class ApiException
	 * \brief Occurs when the engine detects a programmer's error.
	 *
	 * Occurs when the engine detects a programmer's error like null-parameters, wrong call-sequence.
	 */
	struct ApiException : virtual m2exception {
	public:
		M2ETIS_API ApiException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const int line);
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_EXCEPTIONS_H__ */

/**
 * @}
 */
