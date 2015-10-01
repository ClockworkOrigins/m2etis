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

#include "gtest/gtest.h"

// include tests here

// messageing tests

#include "unit/message/test_InternalMessage.h"
#include "unit/message/test_Key.h"
#include "unit/message/test_M2Message.h"
#include "unit/FilterPredicate_test.h"

#include "unit/pubsub/test_PubSubSystem.h"

#if I6E_PLATFORM != I6E_PLATFORM_WIN32
	#include "unit/pubsub/test_Scheduler.h"
	#include "unit/util/test_Clock.h"
#endif
#include "unit/util/test_DoubleBufferQueue.h"
#include "unit/util/test_SegmentTree.h"

int main(int argc, char ** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
