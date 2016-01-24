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
 * @{
 */

#ifndef __M2ETIS_PUBSUB_FILTER_POSITIONEVENT_H__
#define __M2ETIS_PUBSUB_FILTER_POSITIONEVENT_H__

#include <iterator>

#include "boost/serialization/export.hpp"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/variant.hpp"
#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/vector.hpp"

namespace m2etis {
namespace pubsub {
namespace filter {

	class Position {
	public:
		bool operator==(const Position & rhs) const {
			return (rhs.x == this->x && rhs.y == this->y && rhs.region == this->region);
		}

		Position(const std::vector<unsigned char> & payload, int _x = 0, int _y = 0) : region(std::string(payload.begin(), payload.end())), x(_x), y(_y) {}

		Position(const std::string & payload, int _x = 0, int _y = 0) : region(payload), x(_x), y(_y) {}

		Position() {}

		std::string toStr() const {
			return region + ";" + std::to_string(x) + ";" +  std::to_string(y);
		}

		std::string get_region () const { return region; }
		int get_x () const { return x; }
		int get_y () const { return y; }

		std::string issuer = "";
		std::string region = "";
		int x = 0;
		int y = 0;
		std::string direction = "";
		float velocity = 0;
		float acceleration = 0;

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & issuer;
			ar & region;
			ar & x;
			ar & y;
			ar & direction;
			ar & velocity;
			ar & acceleration;
		}
	}; // Class Position

	class Book {
		// example class in Bittner phd
		public:
			Book(const std::string & title, double price, const std::string & condition, int ending) : title_(title), price_(price), condition_(condition), ending_(ending) {}

			std::string title_;
			double price_;
			std::string condition_;
			int ending_;
	}; // class Book

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_POSITIONEVENT_H__ */

/**
 *  @}
 */
