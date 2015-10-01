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

#ifndef __M2ETIS_PUBSUB_ORDER_REMOVEHELPER_H__
#define __M2ETIS_PUBSUB_ORDER_REMOVEHELPER_H__

#include <map>

namespace m2etis {
namespace pubsub {
namespace order {

	template <typename IDType, typename KEYType>
	class RemoveHelper {
	public:
		void dropped(IDType id, const KEYType & rec) {
			missing_[rec].push_back(id);
		}
		
		void dropped(const std::vector<IDType> & droppedIDs, const KEYType & rec) {
			missing_[rec].insert(droppedIDs.begin(), droppedIDs.end());
		}
		
		void clear(const KEYType & rec) {
			missing_[rec].clear();
		}
		
		std::vector<IDType> getDropped(const KEYType & rec) {
			return missing_[rec];
		}

	private:
		std::map<KEYType, std::vector<IDType>> missing_;
	};

} /* namespace order */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ORDER_REMOVEHELPER_H__ */
