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

#ifndef __M2ETIS_CONFIG_ORDER_DETMERGECONFIGTEST_H__
#define __M2ETIS_CONFIG_ORDER_DETMERGECONFIGTEST_H__

namespace m2etis {
namespace config {
namespace order {

	struct DetMergeConfigTest {
		static const unsigned int eps = 50;
		static const unsigned int delta = 10;
		static const unsigned int raster = 1000;
	};

} /* namespace order */
} /* namespace config */
} /* namespace m2etis */

#endif /* __M2ETIS_CONFIG_ORDER_DETMERGECONFIGTEST_H__ */
