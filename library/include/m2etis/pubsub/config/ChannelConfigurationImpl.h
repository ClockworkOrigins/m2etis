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
 * \addtogroup pubsub
 * @ {
 */

#ifndef m2etis_ChannelConfigurationImpl_h
#define m2etis_ChannelConfigurationImpl_h

#include "m2etis/pubsub/config/ChannelTypeDefinitions.h"

namespace m2etis {
    namespace pubsub {

        struct ChannelVector {
            std::vector<ChannelEventInterface *> channels_;
        };

        template<ChannelName... ChannelNames>
        class ChannelManager : public ChannelVector, private ChannelT<ChannelNames>... {
        public:
			ChannelManager(const std::string & ip, const uint16_t port, const std::string & known_hostname, const uint16_t known_hostport, PubSubSystemEnvironment * pssi, const std::vector<std::string> & rootList) :
            ChannelT<ChannelNames>(ip, port, known_hostname, known_hostport, pssi, channels_, rootList)... {
            }

            ~ChannelManager() {
            	for (size_t i = 0; i < channels_.size(); ++i) {
            		delete channels_[i];
            	}
            	channels_.clear();
            }
        };

        class ChannelConfigurationImpl {
        private:
            typedef ChannelManager<M2ETIS_CHANNELLIST> CManager;
            CManager channelmanager_;

        public:
            virtual ~ChannelConfigurationImpl() {}

			ChannelConfigurationImpl(const std::string & ip, const uint16_t port, const std::string & known_hostname, const uint16_t known_hostport, PubSubSystemEnvironment * pssi, const std::vector<std::string> & rootList) :
			channelmanager_(CManager(ip, port, known_hostname, known_hostport, pssi, rootList)) {
            }

            const std::vector<ChannelEventInterface *> & channels() const {
                return channelmanager_.channels_;
            }

            /* data structure to hold the channels which the optimizer creates.
             As the virtual member functions publish and subscribe of a ChannelInterface
             operate on parameters of types BasicM2etisMessage<EventType> or
             BasicDeliverCallbackInterface<EventType>, elements of this data structure have
             to be downcasted into a BasicChannelInterface<EventType> in order to call publish
             or subscribe. The downcast is chosen in favor of more clumsy approaches
             towards covariance (the EventType is special for each ChannelInterface)
             as described in the following paper:
             http:// portal.cbpf.br/attachments/CDI/Exemplo1NT.pdf
             */

            // Furthermore, by using a downcast the current m2etis architecture does not
            // have to be changed (open-closed principle).
        };
    } /* namespace pubsub */
} /* namespace m2etis */
#endif

/**
*  @}
*/
