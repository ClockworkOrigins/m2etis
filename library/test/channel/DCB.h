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

#ifndef __DCB_H__
#define __DCB_H__

#include "m2etis/pubsub/DeliverCallbackInterface.h"

class DCB : public m2etis::pubsub::DeliverCallbackInterface {
public:
	DCB() : _counter(0) {}
	void deliverCallback(const m2etis::message::M2etisMessage::Ptr m) {
		_counter++;
	}

	unsigned int _counter;
};

#endif /* __DCB_H__ */
