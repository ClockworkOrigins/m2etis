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

#ifndef m2etis_GeneratedEventTypes_h
#define m2etis_GeneratedEventTypes_h

#include "m2etis/pubsub/filter/events/Position.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/time_serialize.hpp"

typedef std::vector<unsigned char> CharVectorEventType;
typedef m2etis::pubsub::filter::Position PositionEventType;

class IntegrationTestEventType {
	friend class boost::serialization::access;

public:
	IntegrationTestEventType() : _size() {
	}
	IntegrationTestEventType(size_t size) : _size(size) {
	}

	std::vector<char> _size;
private:

	template <class Archive>
	void serialize(Archive & ar, const unsigned int) {
		ar & _size;
	}
};

#endif
