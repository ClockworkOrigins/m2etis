//
//  GeneratedEventTypes.h
//  m2etis
//
//  Created by Thomas Fischer on 03.05.13.
//
//

#ifndef m2etis_GeneratedEventTypes_h
#define m2etis_GeneratedEventTypes_h

#include "m2etis/pubsub/filter/events/Position.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/time_serialize.hpp"

typedef std::vector<unsigned char> CharVectorEventType;
typedef m2etis::pubsub::filter::Position PositionEventType;

class SimulationEventType {
public:
	SimulationEventType() : _simID(), _simChannel() {
	}

	int _simID;
	int _simChannel;

private:
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & _simID;
			ar & _simChannel;
		}
};

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
