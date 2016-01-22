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
 * \addtogroup util
 * @ {
 */

#ifndef __M2ETIS_UTIL_SEGMENTTREE_H__
#define __M2ETIS_UTIL_SEGMENTTREE_H__

#include <queue>

#include "m2etis/util/Exceptions.h"

namespace m2etis {
namespace util {

	/**
	 * \brief SegmentTree handles integer values and stores segments of set values
	 * e.g. inserting values 2, 3 and 4 results in a segment [2,4]
	 */
	template<typename T>
	class SegmentTree {
	public:
		SegmentTree() : _elements() {
		}

		~SegmentTree() {
			_elements.clear();
		}

		/**
		 * \brief inserts a value into the tree
		 */
		void insert(const T & value) {
			if (contains(value)) {
				return;
			}

			if (_elements.empty()) {
				_elements.push_back(std::make_pair(value, value));
			} else {
				if (value < _elements[0].first) {
					if (_elements[0].first - value == 1) {
						_elements[0].first = value;
					} else {
						_elements.insert(_elements.begin(), std::make_pair(value, value));
					}
				} else if (value > _elements[_elements.size() - 1].second) {
					if (value - _elements[_elements.size() - 1].second == 1) {
						_elements[_elements.size() - 1].second = value;
					} else {
						_elements.push_back(std::make_pair(value, value));
					}
				} else {
					for (unsigned int i = 0; i < _elements.size() - 1; ++i) {
						if (value - _elements[i].second == 1) {
							_elements[i].second = value;

							if (_elements[i + 1].first - value == 1) {
								_elements[i].second = _elements[i + 1].second;
								_elements.erase(_elements.begin() + i + 1);
							}
							break;
						} else if (_elements[i + 1].first - value == 1) {
							_elements[i + 1].first = value;
							break;
						} else if (value > _elements[i].second && value < _elements[i + 1].first) {
							_elements.insert(_elements.begin() + i + 1, std::make_pair(value, value));
							break;
						}
					}
				}
			}
		}

		/**
		 * \brief returns whether value is stored in the tree or not
		 */
		bool contains(const T & value) {
			if (_elements.empty()) {
				return false;
			}

			if (value < _elements[0].first || value > _elements[_elements.size() - 1].second) {
				return false;
			}

			for (unsigned int i = 0; i < _elements.size(); ++i) {
				if (value >= _elements[i].first && value <= _elements[i].second) {
					return true;
				}
			}

			return false;
		}

		/**
		 * \brief returns the amount of segments, is always <= count()
		 */
		size_t size() const {
			return _elements.size();
		}

		/**
		 * \brief returns the amount of stored elements
		 */
		size_t count() const {
			size_t counter = 0;

			for (size_t i = 0; i < _elements.size(); ++i) {
				counter += size_t(_elements[i].second - _elements[i].first + 1);
			}

			return counter;
		}

	private:
		std::vector<std::pair<T, T>> _elements;
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_SEGMENTTREE_H__ */

/**
 * @}
 */
