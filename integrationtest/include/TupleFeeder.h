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

#ifndef __TUPLEFEEDER_H__
#define __TUPLEFEEDER_H__

#include "m2etis/pubsub/DeliverCallbackInterface.h"
#include "m2etis/pubsub/PubSubSystem.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread.hpp"

class TupleFeeder : public m2etis::pubsub::BasicDeliverCallbackInterface<IntegrationTestEventType> {
	enum class NodeType : short {
		SUBSCRIBER,
		PUBLISHER,
		PUBSUBER
	};

public:
	TupleFeeder(const std::string & configFile, const std::string & rendezvousIP, int rendezvousPort, int type, const std::string ownIP, int ownPort);
	~TupleFeeder();
	
	void deliverCallback(const typename m2etis::message::M2Message<IntegrationTestEventType>::Ptr m);

private:
	m2etis::pubsub::PubSubSystem _pubsub;

	std::string _owner;

	NodeType _type;

	m2etis::pubsub::ChannelName _usedChannel;
	size_t _packetSize;
	size_t _maxMessages;

	double _numToSend;

	boost::posix_time::ptime _startTime;
	boost::posix_time::ptime _endTime;
	boost::posix_time::ptime _currentTime;
	long _wholeTime;

	boost::thread _cinThread;

	boost::condition_variable _objCondExecutable;
	boost::mutex _objCondMut;
	boost::unique_lock<boost::mutex> _objCondUniqLock;

	unsigned int _currentID;

	unsigned int _tickTime;

	mutable boost::mutex _lock;

	bool _running;

	std::vector<std::pair<std::pair<std::string, unsigned int>, long>> _latencies;
	std::vector<std::tuple<int, int, double>> _profiling;
	std::vector<std::pair<unsigned int, unsigned int> > _messages;
	std::list<long> _timeList;

	unsigned int _ticksPerSec;
	unsigned int _ticks;

	unsigned int _sent;
	unsigned int _received;

	int parseLine(char * line);

	void initProfiler();

	int getVRAMValue();

	int getPRAMValue();

	double getCurrentValue();

	/**
	 * \brief the main method in the TupleFeeder
	 */
	void run();

	/**
	 * \brief reads the given config file
	 */
	void readConfig(const std::string & file);

	/**
	 * \brief reads input from cin
	 */
	void readCin();

	/**
	 * \brief saves the whole measurements
	 */
	void saveMeasurements();
};

#endif /* __TUPLEFEEDER_H__ */
