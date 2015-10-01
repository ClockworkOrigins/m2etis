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

#ifndef __M2ETIS_M2MESSAGE_TEST_H__
#define __M2ETIS_M2MESSAGE_TEST_H__

#include "m2etis/message/M2Message.h"

typedef m2etis::message::M2Message<std::vector<unsigned char>> MMessage;
typedef m2etis::message::M2Message<std::vector<unsigned char>>::PayloadPtr PP;

TEST(M2MessageTest, Simple) {
	MMessage m1;
	std::string t = "test";
	PP payload(new std::vector<unsigned char>(t.begin(), t.end()));

	m1.payload = payload;
	EXPECT_EQ(payload, m1.payload);
	EXPECT_EQ(t, m1.getPayloadString());
}

#endif /* __M2ETIS_M2MESSAGE_TEST_H__ */
