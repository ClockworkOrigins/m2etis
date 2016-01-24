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

#ifdef WITH_MESSAGECOMPRESSION
	#include "boost/iostreams/filtering_streambuf.hpp"
	#include "boost/iostreams/copy.hpp"
	#include "boost/iostreams/filter/zlib.hpp"
#endif

namespace m2etis {
namespace message {
namespace serialization {

	template<class NetworkType>
	typename NetworkMessage<NetworkType>::Ptr deserializeNetworkMsg(const std::string & msg) {
		std::stringstream objStringStream(msg);

#ifdef WITH_MESSAGECOMPRESSION
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
		out.push(boost::iostreams::zlib_decompressor());
		out.push(objStringStream);
		std::stringstream tmp;
		boost::iostreams::copy(out, tmp);

		boost::archive::text_iarchive objOArchive(tmp, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
#else
		boost::archive::text_iarchive objOArchive(objStringStream, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
#endif

		typename NetworkMessage<NetworkType>::Ptr nm;
		objOArchive >> nm;

		return nm;
	}

	template<class MessageType>
	std::string serializeNetworkMsg(typename MessageType::Ptr msg) {
		std::stringstream objStringStream;
		boost::archive::text_oarchive objOArchive(objStringStream, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		objOArchive << msg;

#ifdef WITH_MESSAGECOMPRESSION
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
		out.push(boost::iostreams::zlib_compressor());
		out.push(objStringStream);
		std::stringstream tmp;
		boost::iostreams::copy(out, tmp);

		return tmp.str();
#else
		return objStringStream.str();
#endif
	}

} /* namespace serialization */
} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_SERIALIZATION_H__ */

/**
 * @}
 */
