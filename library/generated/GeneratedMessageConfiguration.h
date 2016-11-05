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

#ifndef m2etis_MessageConfiguration_h
#define m2etis_MessageConfiguration_h

#include "m2etis/config/GeneratedEventTypes.h"
#include "m2etis/config/DetMergeConfigTest.h"
#include "boost/serialization/serialization.hpp"
#include "boost/serialization/level.hpp"
#include "boost/serialization/tracking.hpp"

BOOST_CLASS_EXPORT(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::NullFilterInfo<CharVectorEventType>) // TODO: (Roland) check if necessary
BOOST_CLASS_IMPLEMENTATION(m2etis::message::NullFilterInfo<CharVectorEventType>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::BruteForceFilterInfo<CharVectorEventType>) // TODO: (Roland) check if necessary
BOOST_CLASS_IMPLEMENTATION(m2etis::message::BruteForceFilterInfo<CharVectorEventType>, boost::serialization::object_serializable)

BOOST_CLASS_EXPORT(m2etis::message::NullOrderInfo)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::NullOrderInfo, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DetMergeOrderInfo<m2etis::config::order::DetMergeConfigTest>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DetMergeOrderInfo<m2etis::config::order::DetMergeConfigTest>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::MTPOrderInfo)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::MTPOrderInfo, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::TCP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::TCP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::UDP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::UDP>>, boost::serialization::object_serializable)

// necessary exports to serialize filters
BOOST_CLASS_EXPORT(m2etis::pubsub::filter::TruePredicate<CharVectorEventType>)
BOOST_CLASS_EXPORT(m2etis::pubsub::filter::Predicate<CharVectorEventType>)
typedef m2etis::pubsub::filter::EqualsAttributeFilter<CharVectorEventType, int> EQUALSCharVectorEventTypeINT;
BOOST_CLASS_EXPORT(EQUALSCharVectorEventTypeINT)
typedef m2etis::pubsub::filter::NotEqualsAttributeFilter<CharVectorEventType, int> NOTEQUALSCharVectorEventTypeINT;
BOOST_CLASS_EXPORT(NOTEQUALSCharVectorEventTypeINT)
typedef m2etis::pubsub::filter::LessThanAttributeFilter<CharVectorEventType, int> LESSTHANCharVectorEventTypeINT;
BOOST_CLASS_EXPORT(LESSTHANCharVectorEventTypeINT)
typedef m2etis::pubsub::filter::GreaterThanAttributeFilter<CharVectorEventType, int> GREATERTHANCharVectorEventTypeINT;
BOOST_CLASS_EXPORT(GREATERTHANCharVectorEventTypeINT)
typedef m2etis::pubsub::filter::GreaterThanAttributeFilter<CharVectorEventType, std::string> GREATERTHANCharVectorEventTypeSTRING;
BOOST_CLASS_EXPORT(GREATERTHANCharVectorEventTypeSTRING)
typedef m2etis::pubsub::filter::EqualsAttributeFilter<CharVectorEventType, std::string> EQUALSCharVectorEventTypeSTRING;
BOOST_CLASS_EXPORT(EQUALSCharVectorEventTypeSTRING)

BOOST_CLASS_EXPORT(m2etis::pubsub::filter::OrExp<CharVectorEventType>)

BOOST_CLASS_EXPORT(m2etis::pubsub::filter::AndExp<CharVectorEventType>)

// Direct / Null UDP

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "0")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "1")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "2")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "3")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "4")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "5")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "6")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "7")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "8")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "9")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "10")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "11")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "12")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "13")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "14")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "15")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "16")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "17")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "18")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "19")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "20")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "21")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "22")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "23")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "24")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "25")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "26")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)




typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "27")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "28")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "29")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "30")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "31")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "32")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "33")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "34")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "35")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "36")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "37")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "38")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "39")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "40")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "41")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "42")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "43")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

#endif
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

#ifndef m2etis_MessageConfiguration_h
#define m2etis_MessageConfiguration_h

#include "m2etis/config/GeneratedEventTypes.h"
#include "m2etis/config/DetMergeConfigTest.h"
#include "boost/serialization/serialization.hpp"
#include "boost/serialization/level.hpp"
#include "boost/serialization/tracking.hpp"

BOOST_CLASS_EXPORT(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::UDP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::TCP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::NullFilterInfo<CharVectorEventType>) // TODO: (Roland) check if necessary
BOOST_CLASS_IMPLEMENTATION(m2etis::message::NullFilterInfo<CharVectorEventType>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::BruteForceFilterInfo<CharVectorEventType>) // TODO: (Roland) check if necessary
BOOST_CLASS_IMPLEMENTATION(m2etis::message::BruteForceFilterInfo<CharVectorEventType>, boost::serialization::object_serializable)

BOOST_CLASS_EXPORT(m2etis::message::NullOrderInfo)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::NullOrderInfo, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DetMergeOrderInfo<m2etis::config::order::DetMergeConfigTest>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DetMergeOrderInfo<m2etis::config::order::DetMergeConfigTest>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::MTPOrderInfo)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::MTPOrderInfo, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::TCP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::TCP>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::UDP>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::UDP>>, boost::serialization::object_serializable)

// necessary exports to serialize filters
BOOST_CLASS_EXPORT(m2etis::pubsub::filter::TruePredicate<CharVectorEventType>)
BOOST_CLASS_EXPORT(m2etis::pubsub::filter::Predicate<CharVectorEventType>)
typedef m2etis::pubsub::filter::EqualsAttributeFilter<CharVectorEventType, int> EQUALSCharVectorEventTypeINT;
BOOST_CLASS_EXPORT(EQUALSCharVectorEventTypeINT)
typedef m2etis::pubsub::filter::NotEqualsAttributeFilter<CharVectorEventType, int> NOTEQUALSCharVectorEventTypeINT;
BOOST_CLASS_EXPORT(NOTEQUALSCharVectorEventTypeINT)
typedef m2etis::pubsub::filter::LessThanAttributeFilter<CharVectorEventType, int> LESSTHANCharVectorEventTypeINT;
BOOST_CLASS_EXPORT(LESSTHANCharVectorEventTypeINT)
typedef m2etis::pubsub::filter::GreaterThanAttributeFilter<CharVectorEventType, int> GREATERTHANCharVectorEventTypeINT;
BOOST_CLASS_EXPORT(GREATERTHANCharVectorEventTypeINT)
typedef m2etis::pubsub::filter::GreaterThanAttributeFilter<CharVectorEventType, std::string> GREATERTHANCharVectorEventTypeSTRING;
BOOST_CLASS_EXPORT(GREATERTHANCharVectorEventTypeSTRING)
typedef m2etis::pubsub::filter::EqualsAttributeFilter<CharVectorEventType, std::string> EQUALSCharVectorEventTypeSTRING;
BOOST_CLASS_EXPORT(EQUALSCharVectorEventTypeSTRING)

BOOST_CLASS_EXPORT(m2etis::pubsub::filter::OrExp<CharVectorEventType>)

BOOST_CLASS_EXPORT(m2etis::pubsub::filter::AndExp<CharVectorEventType>)

// Direct / Null UDP

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "0")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "1")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "2")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "3")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "4")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "5")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "6")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "7")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "8")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "9")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "10")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "11")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "12")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "13")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "14")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "15")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "16")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "17")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "18")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "19")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "20")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "21")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "22")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "23")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "24")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "25")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "26")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)




typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "27")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "28")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "29")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "30")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "31")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "32")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "33")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "34")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "35")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "36")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "37")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "38")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "39")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "40")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "41")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "42")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "43")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

#endif
