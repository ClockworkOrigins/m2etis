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

#ifndef _GLIBCXX_USE_NANOSLEEP
	#define _GLIBCXX_USE_NANOSLEEP // needed for sleep_for, see http://stackoverflow.com/questions/4438084/stdthis-threadsleep-for-and-gcc
#endif

#include <exception>
#include <map>
#include <thread>

#include "m2etis/pubsub/DeliverCallbackInterface.h"
#include "m2etis/pubsub/PubSubSystem.h"

#include "m2etis/pubsub/filter/events/Position.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"

#include "boost/tokenizer.hpp"

#include "clockUtils/argParser/ArgumentParser.h"

enum attribute_names {
	POSITION_X,
	POSITION_Y,
	POSITION_REGION,
	BOOK_TITLE,
	BOOK_PRICE,
	BOOK_CONDITION
};

using m2etis::pubsub::filter::Position;
using m2etis::pubsub::filter::Equals;
using m2etis::pubsub::filter::NotEquals;
using m2etis::pubsub::filter::LessThan;
using m2etis::pubsub::filter::GreaterThan;

std::map<std::string, m2etis::pubsub::ChannelName> chans_ = {
	 {"direct_null_null_null_null_null_null_null_char_tcp", m2etis::pubsub::TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP}
};

// for easier testing purposes and easier switching of event types:
typedef m2etis::pubsub::filter::Position CurrentEventType;
// typedef std::vector<unsigned char> CurrentEventType;

std::map<std::string, std::string> eventTypeNamesMap = { {std::string(typeid(m2etis::pubsub::filter::Position).name()), std::string("position")}, {std::string(typeid(std::vector<unsigned char>).name()), std::string("CharVector")}};


std::string convertEventToString(std::vector<unsigned char> char_vector) {
	return std::string(char_vector.begin(), char_vector.end());
}

std::string convertEventToString(m2etis::pubsub::filter::Position position) {
	return "";
}

class Node: public m2etis::pubsub::BasicDeliverCallbackInterface<CurrentEventType> {
public:
	Node(std::string ip, uint16_t port, std::string hostname, uint16_t host_port, m2etis::pubsub::ChannelName cname, bool sscribe) : _subscribed(sscribe), _pubsub(new m2etis::pubsub::PubSubSystem(ip, port, hostname, host_port, { hostname })), _chan(cname), _port(port) {
        if (_subscribed) {
            _pubsub->subscribe<CurrentEventType>(_chan, *this);
        }
    }

    void publish(CurrentEventType event = CurrentEventType(), int n = 0) {
        std::stringstream a;
		a << _port << ":" << n;
		if (n != 0) {
			a << "__" << n;
		}
		std::string b = a.str();
		std::cout << "publishing... " << a.str() << std::endl;

		std::cout << "content of sent event: " << convertEventToString(event) << std::endl;

		_pubsub->publish(_chan, event);
    }

    // m2etis_node only instantiates one channel.
    // function to avoid compilation errors when trying different event types,
    // should not be called at run time:
    template <typename EventType> void publish(EventType event = EventType(), int n = 0) {
    	return;
    }

    void subscribe() {
        if (!_subscribed) {
            _pubsub->subscribe<CurrentEventType>(_chan, *this);
        } else {
            std::cout << "Already subscribed! Command ignored.";
        }
    }

    void subscribe(boost::shared_ptr<m2etis::pubsub::filter::FilterExp<CurrentEventType> > predicate) {
		std::cout << "filter used to subscribe (as shared_ptr)" << std::string(*predicate)  << "end of filter" << std::endl;
		_pubsub->subscribe<CurrentEventType>(_chan, *this, predicate);
    }

    void subscribe(m2etis::pubsub::filter::FilterExp<CurrentEventType> & predicate) {
		std::cout << "filter used to subscribe" << std::string(predicate)  << "end of filter" << std::endl;
		_pubsub->subscribe<CurrentEventType>(_chan, *this, boost::make_shared<m2etis::pubsub::filter::FilterExp<CurrentEventType> >(predicate));
	}

    // m2etis_node only instantiates one channel.
	// functions to avoid compilation errors when trying different event types (switching CurrentEventType),
	// should not be called at run time:
	template <typename EventType> void subscribe(boost::shared_ptr<m2etis::pubsub::filter::FilterExp<EventType> > predicate) {
		return;
	}

	template <typename EventType> void subscribe(m2etis::pubsub::filter::FilterExp<EventType> predicate) {
		return;
	}

    void unsubscribe() {
        _pubsub->unsubscribe<CurrentEventType>(_chan);
    }

    void unsubscribe(boost::shared_ptr<m2etis::pubsub::filter::FilterExp<CurrentEventType> > predicate) {
		std::cout << "filter used to unsubscribe (as shared_ptr)" << std::string(*predicate)  << "end of filter" << std::endl;
		_pubsub->unsubscribe<CurrentEventType>(_chan, predicate);
    }

    void unsubscribe(m2etis::pubsub::filter::FilterExp<CurrentEventType> & predicate) {
		std::cout << "filter used to unsubscribe" << std::string(predicate)  << "end of filter" << std::endl;
		_pubsub->unsubscribe<CurrentEventType>(_chan, boost::make_shared<m2etis::pubsub::filter::FilterExp<CurrentEventType> >(predicate));
	}

    // m2etis_node only instantiates one channel.
	// functions to avoid compilation errors when trying different event types (switching CurrentEventType),
	// should not be called at run time:
	template <typename EventType> void unsubscribe(boost::shared_ptr<m2etis::pubsub::filter::FilterExp<EventType> > predicate) {
		return;
	}

	template <typename EventType> void unsubscribe(m2etis::pubsub::filter::FilterExp<EventType> predicate) {
		return;
	}

    void deliverCallback(CurrentEventType message) {
    	std::cout << "content of received event: " << convertEventToString(message) << std::endl;
    }

private:
    bool _subscribed;
    m2etis::pubsub::PubSubSystem * _pubsub;
    m2etis::pubsub::ChannelName _chan;
    unsigned int _port;
};

struct Command {
	explicit Command(Node & n) : node(n) {
	}
	virtual ~Command() {}
	virtual void execute() {
		std::cout << "Command not recognized!" << std::endl;
	}

protected:
	Node & node;

	Command & operator=(const Command & other) {
		node = other.node;
		return *this;
	}
};

struct HelpCommand : Command {
    explicit HelpCommand(Node & n) : Command(n) {}

    void execute() {
        std::cout << "x:                    Exit" << std::endl;
        std::cout << "p [count] [delay]:    Publish [count] messages with a delay of [delay] millisecs. Defaults to 1 and 0" << std::endl;
        std::cout << "s:                    Subscribe" << std::endl;
        std::cout << "u:                    Unsubscribe" << std::endl;

        std::cout << std::endl << "the following commands are specific to event type Position: " << std::endl;

		std::cout << "e:                    Subscribe to events, registering two filters " << std::endl;
        std::cout << "f: [region] [x] [y] : Publish position messages with given x value and region value. Defaults to empty string and 0" << std::endl;
        std::cout << "g: [region] [x] [y] : Subscribe to events, registering two filters, restricted to conjunction of equals constraints (e. g. for decision tree filter strategy) " << std::endl;
        std::cout << "y:                  : Unsubscribe from individual filter for decision trees" << std::endl;
        std::cout << "z:                  : Unsubscribe from individual filter specified earlier" << std::endl;
    }
};

struct PublishCommand : Command {
	PublishCommand(Node & n, std::vector<std::string> arg) : Command(n) {
		if (arg.size() == 3) {
			count = std::atoi(arg[1].c_str());
			delay = std::atoi(arg[2].c_str());
		}
	}

	void execute() {
		for (int i = 0; i < count; ++i) {
			node.publish(CurrentEventType(), i + 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
	}

private:
	int count = 1;
	int delay = 0;
};

struct PublishPositionCommand : Command {
	PublishPositionCommand(Node & n, std::vector<std::string> arg) : Command(n) {
		if (arg.size() == 4) {
			position = m2etis::pubsub::filter::Position(arg[1], stoi(arg[2]), stoi(arg[3]));
		} 	else {
			position = m2etis::pubsub::filter::Position("", 0, 0);
		}
	}

	void execute() {
		node.publish(position);
	}

private:
	m2etis::pubsub::filter::Position position;
}; // PublishPositionCommand

struct SubscribeCommand : Command {
	explicit SubscribeCommand(Node & n) : Command(n) {}

	void execute() {
		node.subscribe();
	}
};

struct SubscribeRegionCommand : Command {
	explicit SubscribeRegionCommand(Node & n): Command(n) {}

	void execute() {
		auto subscribe_filter = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >((Equals<Position, std::string>(POSITION_REGION, "Erlangen") || Equals<Position, std::string>(POSITION_REGION, "Nrbg") || Equals<Position, std::string>(POSITION_REGION, "Nrbg") || GreaterThan<Position, std::string>(POSITION_REGION, "u") || LessThan<Position, std::string>(POSITION_REGION, "3")) && (Equals<Position, int>(POSITION_X, 5) || Equals<Position, int>(POSITION_X, 5) || Equals<Position, int>(POSITION_X, 5) || GreaterThan<Position, int>(POSITION_X, 10) || LessThan<Position, int>(POSITION_X, 2)) && (Equals<Position, int>(POSITION_Y, 8) || Equals<Position, int>(POSITION_Y, 8) || Equals<Position, int>(POSITION_Y, 4) || GreaterThan<Position, int>(POSITION_Y, 20) || LessThan<Position, int>(POSITION_Y, 3)) && NotEquals<Position, std::string>(POSITION_REGION, "v") && NotEquals<Position, int>(POSITION_X, 90));

		node.subscribe(subscribe_filter);

		auto subscribe_filter2 = boost::make_shared<m2etis::pubsub::filter::OrExp<Position> >((Equals<Position, int>(POSITION_X, 90) || Equals<Position, std::string>(POSITION_REGION, "Ansbach")));

		node.subscribe(subscribe_filter2);
    }
};

struct SubscribeEqualsPredicatesCommand : Command {
	SubscribeEqualsPredicatesCommand(Node & n, std::vector<std::string> arg) : Command(n) {
		if (arg.size() == 4) {
			x_ = std::atoi(arg[2].c_str());
			y_ = std::atoi(arg[3].c_str());
			region_ = arg[1];
		} else {
			region_ = "";
			x_ = 4;
			y_ = 0;
		}
	}

	void execute() {
		auto subscribe_filter = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(Equals<Position, int>(POSITION_X, x_) && Equals<Position, std::string>(POSITION_REGION, region_) && Equals<Position, int>(POSITION_Y, y_));

		std::cout <<  "filter: " << std::string(*subscribe_filter) << std::endl;

		node.subscribe(subscribe_filter);

		region_= std::string("Erlangen");
		x_ = 0;
		y_ = 20;

		auto subscribe_filter2 = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(Equals<Position, int>(POSITION_X, x_) && Equals<Position, std::string>(POSITION_REGION, region_) && Equals<Position, int>(POSITION_Y, y_));

		std::cout <<  "filter: " << std::string(*subscribe_filter2) << std::endl;

		node.subscribe(subscribe_filter2);
    }

private:
	int x_;
	int y_;
	std::string region_;
};

struct UnsubscribeCommand : Command {
	explicit UnsubscribeCommand(Node & n): Command(n) {}

	void execute() {
		node.unsubscribe();
	}
};

struct UnsubscribeFilterCommand : Command {
	explicit UnsubscribeFilterCommand(Node & n) : Command(n) {}

	void execute() {
		auto unsubscribe_filter = boost::make_shared<m2etis::pubsub::filter::OrExp<Position> >((Equals<Position, int>(POSITION_X, 90) || Equals<Position, std::string>(POSITION_REGION, "Ansbach")));
		node.unsubscribe(unsubscribe_filter);
	}
};

struct UnsubscribeFilterAguileraCommand : Command {
	explicit UnsubscribeFilterAguileraCommand(Node & n): Command(n) {}

	void execute() {
		int x_;
		int y_;
		std::string region_;

		region_= std::string("Erlangen");
		x_ = 0;
		y_ = 20;

		auto unsubscribe_filter = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(Equals<Position, int>(POSITION_X, x_) && Equals<Position, std::string>(POSITION_REGION, region_) && Equals<Position, int>(POSITION_Y, y_));

		std::cout <<  "unsubsceibe filter: " << std::string(*unsubscribe_filter) << std::endl;

		node.unsubscribe(unsubscribe_filter);
    }
};

m2etis::pubsub::ChannelName configureChannel(std::string routing, std::string order, std::string net, std::string filter, std::string partition) {
	std::string channelname = routing + "_" + order + "_" + net + "_" + filter + "_" + partition + "_" +  eventTypeNamesMap[typeid(CurrentEventType).name()];
	if (chans_.find(channelname) == chans_.end()) {
		std::cout << "Couln't find channel for " << channelname << "\n" << "Falling back to " << chans_.begin()->first << std::endl;
		return chans_.begin()->second;
	} else {
		std::cout << "channel configuration: routing: " << routing << " order: " << order << " net: " <<
				" filter: " << filter + " internal event type: " << eventTypeNamesMap[typeid(CurrentEventType).name()] <<
				" partition: " << partition << std::endl;
		return chans_[channelname];
	}
}

Command * dispatch(std::string command, Node & node) {
	boost::tokenizer<> tok(command);
	std::vector<std::string> args;
	for (boost::tokenizer<>::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
		args.push_back(*beg);
	}
	if (args.empty()) {
		return new Command(node);
	}

	if (args[0] == "h") {
		return new HelpCommand(node);
	} else if (args[0] == "p") {
		return new PublishCommand(node, args);
	} else if (args[0] == "u") {
		return new UnsubscribeCommand(node);
	} else if (args[0] == "y") {
		return new UnsubscribeFilterAguileraCommand(node);
	} else if (args[0] == "z") {
		return new UnsubscribeFilterCommand(node);
	} else if (args[0] == "s") {
		return new SubscribeCommand(node);
	} else if (args[0] == "e") {
		return new SubscribeRegionCommand(node);
	} else if (args[0] == "g") {
		return new SubscribeEqualsPredicatesCommand(node, args);
	} else if (args[0] == "f") {
		return new PublishPositionCommand(node, args);
	}
	return new Command(node);
}

int main(int argc, char * argv[]) {
	REGISTER_VARIABLE(std::string, dip, dip, "127.0.0.1", "Destination IP, to connect to for bootstrapping. Defaults to 127.0.0.1");
	REGISTER_VARIABLE(uint16_t, dport, dport, 7000, "Destination port to use for bootstrapping");
	REGISTER_VARIABLE(std::string, lip, lip, "127.0.0.1", "Listening IP for this node. Defaults to 127.0.0.1");
	REGISTER_VARIABLE(uint16_t, lport, lport, 7000, "Listening port for this node. Defaults to 7000.");

	REGISTER_VARIABLE(bool, sub, sub, false, "Flag for automatic subscription.");
	REGISTER_VARIABLE(std::string, net, net, "tcp", "Define which network backend to use. Currently supported: tcp, udp. Defaults to tcp.");
	REGISTER_VARIABLE(std::string, routing, routing, "direct", "Defines which routing strategy to use. Currently supported: spreadit, yoid, nice, direct");
	REGISTER_VARIABLE(std::string, order, order, "null", "Defines which routing strategy to use. Currently supported: null, detmerge, mtp, garcia");
	REGISTER_VARIABLE(std::string, filter, filter, "null", "Defines which filter strategy to use. Currently supported: null, BruteForce, DecisionTree, GeneralBooleanExpressions.");
	REGISTER_VARIABLE(std::string, partition, partition, "null", "Defines which partition strategy to use. Currently supported: null, PartitionFiltersChannel3.");

	bool exit = false;
	if (clockUtils::ClockError::SUCCESS != PARSE_COMMANDLINE()) {
		std::cout << GETLASTPARSERERROR() << std::endl;
		return EXIT_FAILURE;
	}
	if (HELPSET()) {
		std::cout << GETHELPTEXT() << std::endl;
		return EXIT_SUCCESS;
	}
	m2etis::pubsub::ChannelName chname = configureChannel(routing, order, net, filter, partition);
	Node n(lip, lport, dip, dport, chname, sub);
	std::cout << "Node initialized." << std::endl
	<< "Listening on: " << lip << ":" << lport << std::endl
	<< "Bootstrap Node: " << dip << ":" << dport << std::endl
	<< std::endl;

	std::cout << "M2etis Node Console v 0.1" << std::endl << "Press h for help" << std::endl;
	while (!exit) {
		std::string cmd;
		std::cout << "m2etis console:> " << std::endl;
		std::getline(std::cin, cmd);
		if (cmd == "x") {
            exit = true;
		} else {
			Command * c = dispatch(cmd, n);
			c->execute();
			delete c;
		}
	}
	return EXIT_SUCCESS;
}
