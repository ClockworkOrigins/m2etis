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

#include "m2etis/pubsub/config/ChannelConfigurationImpl.h"
#include "m2etis/pubsub/config/ChannelConfiguration.h"

namespace m2etis {
namespace pubsub {

	ChannelConfiguration::ChannelConfiguration(const std::string & ip, const uint16_t port, const std::string & known_hostname, const uint16_t known_hostport, PubSubSystemEnvironment * pssi, const std::vector<std::string> & rootList) : count(CHANNEL_COUNT), impl(new ChannelConfigurationImpl(ip, port, known_hostname, known_hostport, pssi, rootList)) {
	}

	ChannelConfiguration::~ChannelConfiguration() {
		delete impl;
	}

	const std::vector<ChannelEventInterface *> & ChannelConfiguration::channels() const {
		return impl->channels();
	}

} /* namespace pubsub */
} /* namespace m2etis */
