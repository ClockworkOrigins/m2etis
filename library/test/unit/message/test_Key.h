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

#ifndef __M2ETIS_KEY_TEST_H__
#define __M2ETIS_KEY_TEST_H__

#include "m2etis/message/key/Key.h"
#include "m2etis/message/key/providers/IPv4KeyProvider.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

TEST(KeyTest, CtorIPv4) {
    const m2etis::message::Key<m2etis::message::IPv4KeyProvider> k;
    EXPECT_EQ("0.0.0.0:0", k.toStr());
    m2etis::message::Key<m2etis::message::IPv4KeyProvider> * p = new m2etis::message::Key<m2etis::message::IPv4KeyProvider>();
    EXPECT_EQ("0.0.0.0:0", p->toStr());

    m2etis::message::Key<m2etis::message::IPv4KeyProvider> k1("1.2.3.4:12345");
    EXPECT_EQ("1.2.3.4:12345", k1.toStr());

	delete p;
}

TEST(KeyTest, Serialize) {
	m2etis::message::Key<m2etis::message::IPv4KeyProvider> k1("1.2.3.4:12345");
	EXPECT_EQ("1.2.3.4:12345", k1.toStr());

	// serialize
	std::stringstream objStringStream;
	boost::archive::text_oarchive objOArchive(objStringStream);
	objOArchive << k1;
	std::string serialized = objStringStream.str();

	// deserialize
	std::stringstream objStringStream2(serialized);
	boost::archive::text_iarchive objOArchive2(objStringStream2);
	m2etis::message::Key<m2etis::message::IPv4KeyProvider> k2;
	objOArchive2 >> k2;

	EXPECT_EQ("1.2.3.4:12345", k2.toStr());
}

#endif /* __M2ETIS_KEY_TEST_H__ */
