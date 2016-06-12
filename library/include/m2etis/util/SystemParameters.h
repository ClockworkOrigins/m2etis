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

#ifndef __I6ESYSTEMPARAMETERS_H__
#define __I6ESYSTEMPARAMETERS_H__

#include "m2etis/Config.h"

// Global platform specific defines
#define I6E_PLATFORM_WIN32 1
#define I6E_PLATFORM_LINUX 2
#define BOOST_ALL_DYN_LINK
#define WIN32_LEAN_AND_MEAN
// Determine the current platform
#if defined(WIN32 ) || defined(__WIN32__ ) || defined(_WIN32 )
#   define I6E_PLATFORM I6E_PLATFORM_WIN32
#	define WIN32_LEAN_AND_MEAN
#else
#   define I6E_PLATFORM I6E_PLATFORM_LINUX
#endif

// Debug/Release modes
#define I6E_DEBUG	1
#define I6E_RELEASE 2

#if defined(NDEBUG)
#    define I6E_MODE I6E_RELEASE
#else
#    define I6E_MODE I6E_DEBUG
#endif

// Dynamic library import/export macro
// TODO: Finish off all platforms
#ifndef M2ETIS_API
	#if I6E_PLATFORM == I6E_PLATFORM_WIN32
		#ifdef M2ETIS_BUILD_SHARED
			#ifdef m2etis_EXPORTS
				#define M2ETIS_API __declspec(dllexport)
			#else
				#define M2ETIS_API __declspec(dllimport)
			#endif
		#else
			#define M2ETIS_API
		#endif
	#elif I6E_PLATFORM == I6E_PLATFORM_LINUX
		#define M2ETIS_API
	#else
		#define M2ETIS_API
	#endif
#endif

#ifdef _MSC_VER
#   pragma warning(disable : 4251) // dll interface to be used by clients...
#endif

#ifndef M2ETIS_DEPRECATED
	#ifdef __GNUC__
		#define M2ETIS_DEPRECATED __attribute__((deprecated))
	#elif defined(_MSC_VER)
		#define M2ETIS_DEPRECATED __declspec(deprecated)
	#elif defined(_MSC_VER)
		#define M2ETIS_DEPRECATED __attribute__((deprecated))
	#else
		#pragma message("WARNING: You need to implement M2ETIS_DEPRECATED for this compiler")
		#define M2ETIS_DEPRECATED
	#endif
#endif

#endif /* __I6ESYSTEMPARAMETERS_H__ */

/*
 * Documentation for all important macros. As doxygen doesn't offer an overview on all defined macros,
 * a new html page is manually created, that will appear in the documentation. So please document
 * your global macros here. At best in alphabetic order :)
 */
/**
* \page macrodoc Documentation for macros
* These lists contain all important macros.
*
* \section objLike Object-like macros
*
* <table>
* 	<tr>
*  <td> <b> Name </b> </td> <td> <b> Defined in file </b> </td> <td> <b> Brief description </b> </td>
*  </tr>
*  <tr>
*  <td> I6E_API </td> <td> I6eSystemParameters.h </td>
*  <td> This macro contains a compiler directive. It should be written after each class declaration if this class should be
*  	 exported later.
*  </tr>
*  <tr>
*  <td> I6E_MODE </td> <td> I6eSystemParameters.h </td>
*  <td> Is defined either as '1' when running in Debug mode or as '2' when running in Release mode. </td>
*  </tr>
*  <tr>
*  <td> I6E_PLATFORM </td> <td> I6eSystemParameters.h </td>
*  <td> Is defined either as '1' when running in a MS Windows environment or as '2' when running in a Linux environment. </td>
*  </tr>
*  <tr>
*  <td> LNG_GRAPHICS_FRAME_TIME </td> <td> GameEngineController.h </td>
*  <td> This macro sets the frametime the GraphicsController subsystem works with. </td>
*  </tr>
*  <tr>
*  <td> LNG_INPUT_FRAME_TIME </td> <td> GameEngineController.h </td>
*  <td> This macro sets the frametime the InputController subsystem works with. </td>
*  </tr>
*  <tr>
*  <td> LNG_MESSAGING_FRAME_TIME </td> <td> SubSystemController.h </td>
*  <td> This macro sets the frametime the MessagingController subsystem works with. </td>
*  </tr>
*  <tr>
*  <td> LNG_OBJECT_FRAME_TIME </td> <td> GameEngineController.h </td>
*  <td> This macro sets the frametime the ObjectController subsystem works with. </td>
*  </tr>
* </table>
*
* \section funcLike Function-like macros
*
* <table>
* 	<tr>
*  <td> <b> Name </b> </td> <td> <b> Defined in file </b> </td> <td> <b> Brief description </b> </td>
*  </tr>
*  <tr>
*  	<td> CONVERTVARIABLE(objRealVariable, Type, objVariantVariable) </td> <td> MessagingController.h </td>
*  	<td> Used to convert a variable of type ObjectVariable into it's original type.
*  	     Have a look at the MessagingController class description to get more information. </td>
*  </tr>
*  <tr>
*  <td> M2ETIS_LOG_X("Module", "Message") </td> <td> Logger.h </td>
*  <td> This macro is used for the controlled output of error / info / warning / debug messages. X stands
*       for the logging level and can be one of: 'DEBUG', 'INFO', 'WARN', 'ERROR', 'FATAL'.
*       For example: M2ETIS_LOG_WARN("PhysicsController", "Erratic Gravity!") </td>
*  </tr>
*  <tr>
*  <td> M2ETIS_LOG_SETLEVEL(level) </td> <td> Logger.h </td>
*  <td> Sets the new LogLevel (i6utils::Logger::LogLevel). Everything at least as important as 'level' is logged.
*       For example: M2ETIS_LOG_SETLEVEL(i6utils::Logger::INFO) </td>
*  </tr>
* 	<tr>
*		<td> REGISTERMESSAGETYPE(strMessageTypeName, ptrMessageMethod) </td> <td> MessagingController.h </td>
*		<td> This macro should be used to register message types at the MessagingController.
*		     Have a look at its class description to get more information. </td>
*  </tr>
*  <tr>
*  	<td> RETURNIFVARIABLEISNULL(objVariable) </td> <td> MessagingController.h </td>
*  	<td> Macro to check of an extracted variable of a message is null. Should be used just before the CONVERTVARIABLE
*  	     macro. Have a look at the MessagingController class description to get more information. </td>
*  </tr>
*  <tr>
*  	<td> UNREGISTERMESSAGETYPE(strMessageTypeName) </td> <td> MessagingController.h </td>
*  	<td> Deletes a registration of a message type.
*  	     Have a look at the MessagingController class description to get more information. </td>
*  </tr>
* </table>
*
*/
