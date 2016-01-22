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

#ifndef __M2ETIS_WRAPPER_CHIMERA_CHIMERARESPONSIBLE_H__
#define __M2ETIS_WRAPPER_CHIMERA_CHIMERARESPONSIBLE_H__

#include <cmath>
#include <string>

#include "m2etis/wrapper/chimera/ChimeraWrapperImpl.h"


extern "C" {
	#include "chimera/key.h"
}

// that's defined in key.c
extern "C" char * sha1_keygen(char * key, size_t digest_size, char * digest);


namespace m2etis {
namespace wrapper {
namespace chimera {

	class ChimeraResponsible {
	public:
		explicit ChimeraResponsible(const std::string & topic) : self_(), root_() {
			char * got = sha1_keygen(const_cast<char*>(topic.c_str()), topic.size(), 0);
			root_ = std::string(got);
			free(got); // yes, I know. But it's created via malloc inside sha1_keygen.
		}

		virtual ~ChimeraResponsible() {}

		void setSelf(const std::string & self) {
			self_ = self;
		}

		bool isResponsible() const {
			return wrapper::chimera::ChimeraWrapperImpl::GetSingletonPtr()->isResponsibleFor(getRoot(), self_);
		}

		// So, now calculate the SHA for the given topic_name
		std::string getRoot() const {
			return root_;
		}

	private:
		std::string self_;
		std::string root_;
	};

} /* namespace chimera */
} /* namespace wrapper */
} /* namespace m2etis */

#endif /* __M2ETIS_WRAPPER_CHIMERA_CHIMERARESPONSIBLE_H__ */
