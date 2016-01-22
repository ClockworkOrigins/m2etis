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

#include "m2etis/pubsub/PubSubSystem.h"

void createandkillm2etisheap() {
    m2etis::pubsub::PubSubSystem * p1 = new m2etis::pubsub::PubSubSystem("1.0.0.1", 12345, "1.0.0.1", 12345, {"1.0.0.1"});

	delete p1;
}

void createandkillm2etisstack() {
    m2etis::pubsub::PubSubSystem p("1.0.0.1", 12345, "1.0.0.1", 12345, {"1.0.0.1"});
}

TEST(PubSubSystem, Lifecycle) {
    createandkillm2etisheap();
    createandkillm2etisheap();
    createandkillm2etisstack();
    createandkillm2etisstack();
}
