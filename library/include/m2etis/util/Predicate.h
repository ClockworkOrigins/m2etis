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

/**
 * \addtogroup util
 * @ {
 */

#ifndef __M2ETIS_UTIL_PREDICATE_H__
#define __M2ETIS_UTIL_PREDICATE_H__

namespace m2etis {
namespace util {

	// TODO: (Daniel) what's this for? filtering?
	template<typename EventType>
	class Predicate : std::unary_function<type, bool> {
		std::function<bool(M2etisMessage<EventType>)> expr;

	public:
		explicit Predicate(std::function<bool(M2etisMessage<EventType>)> x) : expr(x) {
		}

		/**
		 <#description#>

		 @param <#parameter#>
		 @returns <#retval#>
		 @exception <#throws#>
		 */
		bool operator()(M2etisMessage<EventType> arg) {
			return expr(arg);
		}
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_PREDICATE_H__ */

/**
 * @}
 */
