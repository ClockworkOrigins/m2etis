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


#ifndef __M2ETIS_PUBSUB_RENDEZVOUS_NULLRENDEZVOUS_H__
#define __M2ETIS_PUBSUB_RENDEZVOUS_NULLRENDEZVOUS_H__

#include "m2etis/pubsub/rendezvous/BaseRendezvous.h"

namespace m2etis {
namespace pubsub {
namespace rendezvous {

	class NullRendezvous : public BaseRendezvous {
	public:
		explicit NullRendezvous(const std::vector<std::string> & rootList) : BaseRendezvous(rootList), _position() {
		}
		~NullRendezvous() {}

		std::string getRoot() {
			_position = _position % _rootList.size();
			return _rootList[_position++];
		}

	private:
		unsigned int _position;
	};

} /* namespace rendezvous */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_RENDEZVOUS_NULLRENDEZVOUS_H__ */
