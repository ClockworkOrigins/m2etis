/*
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
 * \addtogroup pubsub
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_FILTER_ATTRIBUTEACCESSORBASIC_H__
#define __M2ETIS_PUBSUB_FILTER_ATTRIBUTEACCESSORBASIC_H__

namespace m2etis {
namespace pubsub {
namespace filter {

	// maps attribute identifier to attribute value
	class AttributeAccessor_Basic  {
	public:
		virtual ~AttributeAccessor_Basic() {}
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_ATTRIBUTEACCESSORBASIC_H__ */

/**
 *  @}
 */
