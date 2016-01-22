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

#include "TupleFeeder.h"

#include "sys/times.h"
#include "sys/vtimes.h"

#include "boost/property_tree/info_parser.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/thread.hpp"

static clock_t lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;

TupleFeeder::TupleFeeder(const std::string & configFile, const std::string & rendezvousIP, int rendezvousPort, int type, const std::string ownIP, int ownPort) : _pubsub(ownIP, static_cast<unsigned short>(ownPort), rendezvousIP, static_cast<unsigned short>(rendezvousPort), {rendezvousIP}), _owner(ownIP), _type(static_cast<NodeType>(type)), _packetSize(), _maxMessages(), _startTime(boost::posix_time::microsec_clock::universal_time()), _currentTime(_startTime), _wholeTime(0), _cinThread(boost::bind(&TupleFeeder::readCin, this)), _objCondExecutable(), _objCondMut(), _objCondUniqLock(_objCondMut), _currentID(0), _tickTime(), _lock(), _running(true), _latencies(), _profiling(), _messages(), _timeList(), _ticksPerSec(), _ticks(), _sent(), _received() {
	readConfig(configFile);
	initProfiler();
	run();
}

TupleFeeder::~TupleFeeder() {
}

void TupleFeeder::run() {
	_objCondExecutable.wait(_objCondUniqLock);

	if (_type == NodeType::SUBSCRIBER || _type == NodeType::PUBSUBER) {
		_pubsub.subscribe<IntegrationTestEventType>(_usedChannel, *this);
	}

	_objCondExecutable.wait(_objCondUniqLock);

	IntegrationTestEventType e(_packetSize);

	m2etis::message::M2Message<IntegrationTestEventType>::Ptr msg = _pubsub.createMessage<IntegrationTestEventType>(_usedChannel, e);

	if (_type == NodeType::PUBLISHER || _type == NodeType::PUBSUBER) {
		while (_running) {
			boost::posix_time::ptime tickStart = boost::posix_time::microsec_clock::universal_time();
			for (int i = 0; i < _numToSend; i++) {
				_pubsub.publish<IntegrationTestEventType>(_usedChannel, msg);
				_sent++;

				if (_sent == _maxMessages) {
					_running = false;
					break;
				}
			}

			boost::this_thread::sleep(boost::posix_time::milliseconds(1000 - static_cast<long>(boost::posix_time::time_period(tickStart, boost::posix_time::microsec_clock::universal_time()).length().total_milliseconds())));
		}
	}

	if ((_type == NodeType::SUBSCRIBER || _type == NodeType::PUBSUBER) && _running) {
		_objCondExecutable.wait(_objCondUniqLock);
	}

	_endTime = boost::posix_time::microsec_clock::universal_time();

	saveMeasurements();
}

void TupleFeeder::deliverCallback(const typename m2etis::message::M2Message<IntegrationTestEventType>::Ptr m) {
	_received++;

	if (_received == _maxMessages) {
		_running = false;
		long l = static_cast<long>(boost::posix_time::time_period(_startTime, boost::posix_time::microsec_clock::universal_time()).length().total_microseconds()) / 1000.0;
		_timeList.push_back(l);
		_wholeTime += l;
		_objCondExecutable.notify_all();
	} else if (_received % size_t(_numToSend) == 0) {
		long l = static_cast<long>(boost::posix_time::time_period(_startTime, boost::posix_time::microsec_clock::universal_time()).length().total_microseconds()) / 1000.0;
		_timeList.push_back(l);
		_wholeTime += l;
	} else if (_received % size_t(_numToSend) == 1) {
		_startTime = boost::posix_time::microsec_clock::universal_time();
	}
}

int TupleFeeder::parseLine(char * line){
	int i = strlen(line);
	while (*line < '0' || *line > '9') {
		line++;
	}
	line[i - 3] = '\0';
	i = atoi(line);
	return i;
}


int TupleFeeder::getVRAMValue() { // Note: this value is in KB!
	FILE * file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[128];

	while (fgets(line, 128, file) != nullptr) {
		if (strncmp(line, "VmSize:", 7) == 0) {
			result = parseLine(line);
			break;
		}
	}
	fclose(file);
	return result;
}

int TupleFeeder::getPRAMValue() { // Note: this value is in KB!
	FILE * file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[128];

	while (fgets(line, 128, file) != nullptr) {
		if (strncmp(line, "VmRSS:", 6) == 0) {
			result = parseLine(line);
			break;
		}
	}
	fclose(file);
	return result;
}

void TupleFeeder::initProfiler() {
	FILE * file;
	struct tms timeSample;
	char line[128];

	lastCPU = times(&timeSample);
	lastSysCPU = timeSample.tms_stime;
	lastUserCPU = timeSample.tms_utime;

	file = fopen("/proc/cpuinfo", "r");
	numProcessors = 0;
	while(fgets(line, 128, file) != nullptr) {
		if (strncmp(line, "processor", 9) == 0) {
			numProcessors++;
		}
	}
	fclose(file);
}

double TupleFeeder::getCurrentValue() {
	struct tms timeSample;
	clock_t now;
	double percent;

	now = times(&timeSample);
	if (now <= lastCPU || timeSample.tms_stime < lastSysCPU || timeSample.tms_utime < lastUserCPU) {
		//Overflow detection. Just skip this value.
		percent = -1.0;
	} else {
		percent = (timeSample.tms_stime - lastSysCPU) + (timeSample.tms_utime - lastUserCPU);
		percent /= (now - lastCPU);
		percent /= numProcessors;
		percent *= 100;
	}
	lastCPU = now;
	lastSysCPU = timeSample.tms_stime;
	lastUserCPU = timeSample.tms_utime;

	return percent;
}

void TupleFeeder::readConfig(const std::string & file) {
	boost::property_tree::ptree pt;
	boost::property_tree::read_info(file, pt);

	unsigned int usedChannel = pt.get("usedChannel", usedChannel);
	_usedChannel = static_cast<m2etis::pubsub::ChannelName>(usedChannel);
	_numToSend = pt.get("numToSend", _numToSend);
	_packetSize = pt.get("packetSize", _packetSize);
	_maxMessages = pt.get("maxMessages", _maxMessages);

	_tickTime = 1000000 / _numToSend;

	_ticksPerSec = _numToSend;
}

void TupleFeeder::readCin() {
	std::string s;
	while (true) {
		std::getline(std::cin, s);

		if (s == "s") {
			_objCondExecutable.notify_all();
		} else if (s == "p") {
			_objCondExecutable.notify_all();
		} else if (s == "x") {
			_lock.lock();
			_running = false;
			_lock.unlock();
			break;
		}
	}
}

void TupleFeeder::saveMeasurements() {
	std::fstream f(_owner + "_messages.xml", std::ios_base::out);
	for (long l : _timeList) {
		f << "<event type=\"deliver time\">" << l << "</event>" << std::endl;
	}
	f << "<event type=\"average time\">" << _wholeTime / (_maxMessages / _numToSend) << "</event>" << std::endl;
	f.close();

	std::fstream f2(_owner + "_profiling.xml", std::ios_base::out);
	for (std::tuple<int, int, double> p : _profiling) {
		f2 << "<profiling><PRAM>" << std::get<0>(p) << "</PRAM><VRAM>" << std::get<1>(p) << "</VRAM><CPU>" << std::get<2>(p) << "</CPU></profiling>" << std::endl;
	}
	f2.close();
}
