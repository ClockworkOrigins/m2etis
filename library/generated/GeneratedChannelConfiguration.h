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

#ifndef M2ETIS_GENERATEDCHANNELCONFIGURATION_H
#define M2ETIS_GENERATEDCHANNELCONFIGURATION_H


}}
// Todo: refactor (namespace opened by ChannelTypeDefinitions.h closed)
// namespace m2etis pubsub closed because of the following includes:

#include "m2etis/config/GeneratedEventTypes.h"
#include "m2etis/config/DetMergeConfigTest.h"

namespace m2etis {
namespace pubsub {


#include <map>
#include <vector>

#include "m2etis/pubsub/filter/AttributeTypeInformation.h"
#include "m2etis/pubsub/filter/AttributeAccessorBasic.h"
#include "m2etis/pubsub/filter/AttributeAccessor.h"


using ::m2etis::pubsub::filter::AttributeName;
using ::m2etis::pubsub::filter::AttributeAccessor_Basic;
using ::m2etis::pubsub::filter::AttributeAccessor;


// needed for serializing attribute value accessors for filters:
// maps attribute identifier to attribute value getter
// todo: maybe move to better place
enum attribute_names {
	  POSITION_X, POSITION_Y, POSITION_REGION, BOOK_TITLE, BOOK_PRICE, BOOK_CONDITION
  };

M2ETIS_API std::map<AttributeName, std::shared_ptr<AttributeAccessor_Basic>> attributes_accessor_map = {
		{POSITION_Y, std::make_shared<AttributeAccessor<Position, int>> (
				[] (const Position& position)->int {return position.get_y();} )
		},
		{POSITION_X, std::make_shared<AttributeAccessor<Position, int>> (
				[] (const Position& position)->int {return position.get_x();} )
		},
		{POSITION_REGION, std::make_shared<AttributeAccessor<Position, std::string>> (
				[] (const Position& position)->std::string {return position.get_region();} )
		},
		{BOOK_TITLE, std::make_shared<AttributeAccessor<Book, std::string>> (
				[] (const Book& book)->std::string {return book.title_;} )
		},
		{BOOK_CONDITION, std::make_shared<AttributeAccessor<Book, std::string>> (
						[] (const Book& book)->std::string {return book.condition_;} )
		},
		{BOOK_PRICE, std::make_shared<AttributeAccessor<Book, double>> (
						[] (const Book& book)->double {return book.price_;} )
		}
};



#ifdef WITH_SIM

// SIM_Spreadit_Null_OMNET
typedef Channel<
ChannelType<
SpreaditRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, NullOrder<net::NetworkType<net::OMNET>>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Spreadit_Null_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Spreadit_Null_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Spreadit_Null_OMNET_Null_Null_SimulationEventType(SIM_Spreadit_Null_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Spreadit_DetMerge_OMNET
typedef Channel<
ChannelType<
SpreaditRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, DetMergeOrder<net::NetworkType<net::OMNET>, m2etis::pubsub::order::DetMergeConfig>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Spreadit_DetMerge_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Spreadit_DetMerge_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Spreadit_DetMerge_OMNET_Null_Null_SimulationEventType(SIM_Spreadit_DetMerge_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Spreadit_MTP_OMNET
typedef Channel<
ChannelType<
SpreaditRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, MTPOrder<net::NetworkType<net::OMNET>, 1000000, LateDeliver::DROP>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Spreadit_MTP_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Spreadit_MTP_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Spreadit_MTP_OMNET_Null_Null_SimulationEventType(SIM_Spreadit_MTP_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Spreadit_GMS_OMNET
typedef Channel<
ChannelType<
SpreaditRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, GMSOrder<net::NetworkType<net::OMNET>, 1000000>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Spreadit_GMS_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Spreadit_GMS_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Spreadit_GMS_OMNET_Null_Null_SimulationEventType(SIM_Spreadit_GMS_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Direct_Null_OMNET
typedef Channel<
ChannelType<
DirectRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, NullOrder<net::NetworkType<net::OMNET>>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Direct_Null_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Direct_Null_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Direct_Null_OMNET_Null_Null_SimulationEventType(SIM_Direct_Null_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Direct_MTP_OMNET
typedef Channel<
ChannelType<
DirectRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, MTPOrder<net::NetworkType<net::OMNET>, 1000000, LateDeliver::DROP>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Direct_MTP_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Direct_MTP_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Direct_MTP_OMNET_Null_Null_SimulationEventType(SIM_Direct_MTP_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Direct_DetMerge_OMNET
typedef Channel<
ChannelType<
DirectRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, DetMergeOrder<net::NetworkType<net::OMNET>, DetMergeConfig>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Direct_DetMerge_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Direct_DetMerge_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Direct_DetMerge_OMNET_Null_Null_SimulationEventType(SIM_Direct_DetMerge_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Direct_GMS_OMNET
typedef Channel<
ChannelType<
DirectRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, GMSOrder<net::NetworkType<net::OMNET>, 1000000>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Direct_GMS_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Direct_GMS_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Direct_GMS_OMNET_Null_Null_SimulationEventType(SIM_Direct_GMS_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_DirectBroadcast_Null_OMNET
typedef Channel<
ChannelType<
DirectBroadcastRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, NullOrder<net::NetworkType<net::OMNET>>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_DirectBroadcast_Null_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_DirectBroadcast_Null_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_DirectBroadcast_Null_OMNET_Null_Null_SimulationEventType(SIM_DirectBroadcast_Null_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_DirectBroadcast_MTP_OMNET
typedef Channel<
ChannelType<
DirectBroadcastRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, MTPOrder<net::NetworkType<net::OMNET>, 1000000, LateDeliver::DROP>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_DirectBroadcast_MTP_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_DirectBroadcast_MTP_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_DirectBroadcast_MTP_OMNET_Null_Null_SimulationEventType(SIM_DirectBroadcast_MTP_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_DirectBroadcast_DetMerge_OMNET
typedef Channel<
ChannelType<
DirectBroadcastRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, DetMergeOrder<net::NetworkType<net::OMNET>, DetMergeConfig>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_DirectBroadcast_DetMerge_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_DirectBroadcast_DetMerge_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_DirectBroadcast_DetMerge_OMNET_Null_Null_SimulationEventType(SIM_DirectBroadcast_DetMerge_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_DirectBroadcast_GMS_OMNET
typedef Channel<
ChannelType<
DirectBroadcastRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, GMSOrder<net::NetworkType<net::OMNET>, 1000000>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_DirectBroadcast_GMS_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_DirectBroadcast_GMS_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_DirectBroadcast_GMS_OMNET_Null_Null_SimulationEventType(SIM_DirectBroadcast_GMS_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Scribe_Null_OMNET
typedef Channel<
ChannelType<
ScribeRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, MTPOrder<net::NetworkType<net::OMNET>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, AckDeliver<net::NetworkType<net::OMNET>, 5, m2etis::pubsub::deliver::Amount::EXACTLY_ONCE>
, NullPersistence
, NullValidity
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Scribe_Null_OMNET_Null_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Scribe_Null_OMNET_Null_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Scribe_Null_OMNET_Null_Null_SimulationEventType(SIM_Scribe_Null_OMNET_Null_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Direct_Null_OMNET_Ack_Null
typedef Channel<
ChannelType<
DirectRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, NullOrder<net::NetworkType<net::OMNET>>
, AckDeliver<net::NetworkType<net::OMNET>, 5, m2etis::pubsub::deliver::Amount::EXACTLY_ONCE>
, NullPersistence
, NullValidity, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Direct_Null_OMNET_Ack_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Direct_Null_OMNET_Ack_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Direct_Null_OMNET_Ack_Null_SimulationEventType(SIM_Direct_Null_OMNET_Ack_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

// SIM_Spreadit_Null_OMNET
typedef Channel<
ChannelType<
SpreaditRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, NullOrder<net::NetworkType<net::OMNET>>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, TimeValidity<0, 5000000>
, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Spreadit_Null_OMNET_Null_Time_SimulationEventType;

template <>
struct ChannelT<SIM_Spreadit_Null_OMNET_Null_Time_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Spreadit_Null_OMNET_Null_Time_SimulationEventType(SIM_Spreadit_Null_OMNET_Null_Time_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
ChannelType<
DirectRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, NullOrder<net::NetworkType<net::OMNET>>
, NullDeliver<net::NetworkType<net::OMNET>>
, NullPersistence
, NullValidity
, DirectBroadcastPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Direct_Null_OMNET_Null_Null_DirectBroadcast_SimulationEventType;

template <>
struct ChannelT<SIM_Direct_Null_OMNET_Null_Null_DirectBroadcast_Simulation> {
    ChannelT(const std::string & ip, const int port, const std::string & known_hostname, const int known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Direct_Null_OMNET_Null_Null_DirectBroadcast_SimulationEventType(SIM_Direct_Null_OMNET_Null_Null_DirectBroadcast_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};



typedef Channel<
ChannelType<
DirectRouting<net::NetworkType<net::OMNET>>
, NullFilter<SimulationEventType, net::NetworkType<net::OMNET>>
, NullOrder<net::NetworkType<net::OMNET>>
, NackDeliver<net::NetworkType<net::OMNET>, 5, m2etis::pubsub::deliver::Amount::EXACTLY_ONCE>
, NullPersistence
, NullValidity, NullPartition<net::NetworkType<net::OMNET>, SimulationEventType>
, NullSecurity, NullRendezvous
>,
net::NetworkType<net::OMNET>,
SimulationEventType
> SIM_Direct_Null_OMNET_Nack_Null_SimulationEventType;

template <>
struct ChannelT<SIM_Direct_Null_OMNET_Nack_Null_Simulation> {
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new SIM_Direct_Null_OMNET_Nack_Null_SimulationEventType(SIM_Direct_Null_OMNET_Nack_Null_Simulation, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

#else

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , GMSOrder<net::NetworkType<net::TCP>, 1000000>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , GMSOrder<net::NetworkType<net::UDP>, 1000000>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , GMSOrder<net::NetworkType<net::TCP>, 1000000>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , GMSOrder<net::NetworkType<net::UDP>, 1000000>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , GMSOrder<net::NetworkType<net::TCP>, 1000000>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , GMSOrder<net::NetworkType<net::UDP>, 1000000>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , GMSOrder<net::NetworkType<net::TCP>, 1000000>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , GMSOrder<net::NetworkType<net::UDP>, 1000000>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , GMSOrder<net::NetworkType<net::TCP>, 1000000>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , GMSOrder<net::NetworkType<net::UDP>, 1000000>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , GMSOrder<net::NetworkType<net::TCP>, 1000000>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , NullFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , GMSOrder<net::NetworkType<net::UDP>, 1000000>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , NullOrder<net::NetworkType<net::TCP>>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , DetMergeOrder<net::NetworkType<net::TCP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , DetMergeOrder<net::NetworkType<net::UDP>, m2etis::config::order::DetMergeConfigTest>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , MTPOrder<net::NetworkType<net::TCP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , MTPOrder<net::NetworkType<net::UDP>, 1000000, LateDeliver::DROP>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , GMSOrder<net::NetworkType<net::TCP>, 1000000>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPType( TEST_Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , GMSOrder<net::NetworkType<net::UDP>, 1000000>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , NullPartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPType( TEST_Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::TCP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::TCP>>
        , GMSOrder<net::NetworkType<net::TCP>, 1000000>
        , NullDeliver<net::NetworkType<net::TCP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::TCP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::TCP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPType( TEST_Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        SpreaditRouting<net::NetworkType<net::UDP>>
        , BruteForceFilter<CharVectorEventType, net::NetworkType<net::UDP>>
        , GMSOrder<net::NetworkType<net::UDP>, 1000000>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , BruteForcePartition<net::NetworkType<net::UDP>, CharVectorEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    CharVectorEventType
> TEST_Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPType;

template <>
struct ChannelT<TEST_Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPType( TEST_Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

typedef Channel<
    ChannelType<
        DirectRouting<net::NetworkType<net::UDP>>
        , NullFilter<IntegrationTestEventType, net::NetworkType<net::UDP>>
        , NullOrder<net::NetworkType<net::UDP>>
        , NullDeliver<net::NetworkType<net::UDP>>
        , NullPersistence
        , NullValidity
        , DirectBroadcastPartition<net::NetworkType<net::UDP>, IntegrationTestEventType>
        , NullSecurity, NullRendezvous
    >,
    net::NetworkType<net::UDP>,
    IntegrationTestEventType
> TEST_Direct_Integration_UDPType;

template <>
struct ChannelT<TEST_Direct_Integration_UDP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new TEST_Direct_Integration_UDPType(TEST_Direct_Integration_UDP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

#endif

#endif
