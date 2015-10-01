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

/**
 * \addtogroup net
 * @ {
 */

#ifndef __M2ETIS_NET_NODEHANDLE_H__
#define __M2ETIS_NET_NODEHANDLE_H__

#include <list>
#include <sstream>
#include <string>

#include "m2etis/message/key/Key.h"
#include "m2etis/message/key/providers/KeyProviders.h"

#include "boost/shared_ptr.hpp"

namespace m2etis {
namespace net {

	/**
	 * \ingroup net
	 *
	 * \class NodeHandle
	 * \brief NodeHandle in different structured p2p-systems.
	 *
	 * Long Desc
	 *
	 */
    template<class NetworkType>
	class NodeHandle {
	public:
		NodeHandle() : key_(), hostname_(), port_(0) {}
		NodeHandle(typename NetworkType::Key key, const std::string & hostname, const uint16_t port) : key_(key), hostname_(hostname), port_(port) {
		}
		~NodeHandle() {}

		typename NetworkType::Key key_;
		std::string hostname_;
		uint16_t port_;

		std::string to_str() const {
			std::stringstream ss;
			ss << key_.toStr() << "\t(" << hostname_ << ":" << port_ << ")";
			return ss.str();
		}

		typedef boost::shared_ptr<const NodeHandle<NetworkType>> Ptr_const;
		typedef boost::shared_ptr<NodeHandle<NetworkType>> Ptr;
		typedef std::list<Ptr> PtrList;
	};

} /* namespace net */
} /* namespace m2etis */

#endif /* __M2ETIS_NET_NODEHANDLE_H__ */
