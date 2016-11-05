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
 * \addtogroup message
 * @{
 */

#ifndef __M2ETIS_MESSAGE_SERIALIZATION_H__
#define __M2ETIS_MESSAGE_SERIALIZATION_H__

#include "m2etis/message/InternalMessage.h"

#include "m2etis/pubsub/config/ChannelType.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

namespace m2etis {
namespace message {
namespace serialization {

	template<class NetworkType>
	typename NetworkMessage<NetworkType>::Ptr deserializeNetworkMsg(const std::string & msg) {
		std::stringstream objStringStream(msg);
		typename NetworkMessage<NetworkType>::Ptr nm;
		try {
			boost::archive::text_iarchive objOArchive(objStringStream, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
			objOArchive >> nm;
		} catch (boost::archive::archive_exception & e) {
			std::cout << e.what() << std::endl;
		}

		return nm;
	}

	template<class MessageType>
	std::string serializeNetworkMsg(typename MessageType::Ptr msg) {
		std::stringstream objStringStream;
		try {
			boost::archive::text_oarchive objOArchive(objStringStream, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
			objOArchive << msg;
		} catch (boost::archive::archive_exception & e) {
			std::cout << e.what() << std::endl;
		}
		return objStringStream.str();
	}

} /* namespace serialization */
} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_SERIALIZATION_H__ */

/**
 * @}
 */
