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

#include <ifaddrs.h>

#include "TupleFeeder.h"

#include "boost/asio.hpp"

int main(int argc, char ** argv) {
	argc--;
	argv++;
	
	if (argc != 5) {
		std::cerr << "Five parameters expected: config file, rendezvous IP, rendezvous port, own port and node type" << std::endl;
		return 1;
	}
	
	struct ifaddrs * ifAddrStruct = nullptr;
	struct ifaddrs * ifa = nullptr;
	void * tmpAddrPtr = nullptr;
	getifaddrs(&ifAddrStruct);
	
	std::string ownIP;

	for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
		if (ifa ->ifa_addr->sa_family == AF_INET) { // check it is IP4
			tmpAddrPtr = &(reinterpret_cast<struct sockaddr_in *>(ifa->ifa_addr))->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

			if (std::strcmp(ifa->ifa_name , "eth0") == 0) {
				ownIP = std::string(addressBuffer);
				break;
			}
		}
	}
	
	TupleFeeder tf(argv[0], argv[1], std::atoi(argv[2]), std::atoi(argv[4]), ownIP, std::atoi(argv[3]));
	return 0;
}
