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

// The sequence of following statements is important. Do not reorder!!!
#ifndef __M2ETIS_PUBSUB_CHANNELCONFIGURATION_H__
#define __M2ETIS_PUBSUB_CHANNELCONFIGURATION_H__


namespace m2etis {
    namespace pubsub {

        // Forward Decls
        class ChannelConfigurationImpl;
        struct ChannelEventInterface;
        class PubSubSystemEnvironment;

		class M2ETIS_API ChannelConfiguration {
        private:
            ChannelConfigurationImpl * impl;

        public:
            uint8_t count;

            virtual ~ChannelConfiguration();

            ChannelConfiguration(const std::string & ip, const uint16_t port, const std::string & known_hostname, const uint16_t known_hostport, PubSubSystemEnvironment * pssi, const std::vector<std::string> & rootList);

            const std::vector<ChannelEventInterface *> & channels() const;
        };
    }
}

#endif /* __M2ETIS_PUBSUB_CHANNELCONFIGURATION_H__ */

/**
 *  @}
 */
