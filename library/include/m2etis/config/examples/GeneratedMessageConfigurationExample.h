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

#ifndef m2etis_MessageConfigurationExample_h
#define m2etis_MessageConfigurationExample_h

#include "m2etis/config/GeneratedEventTypes.h"
#include "m2etis/config/DetMergeConfigTest.h"
#include "boost/serialization/serialization.hpp"
#include "boost/serialization/level.hpp"
#include "boost/serialization/tracking.hpp"

#ifdef WITH_SIM
BOOST_CLASS_EXPORT(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::HierarchicalSpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::HierarchicalSpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::SpreadItRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::ScribeRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::ScribeRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::NullOrderInfo)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::NullOrderInfo, boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::MTPOrderInfo, boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT(m2etis::message::DetMergeOrderInfo<m2etis::pubsub::order::DetMergeConfig>)
BOOST_CLASS_EXPORT(m2etis::message::MTPOrderInfo)
BOOST_CLASS_EXPORT(m2etis::message::GMSOrderInfo<m2etis::net::NetworkType<m2etis::net::OMNET>>)
// Deliver
BOOST_CLASS_EXPORT(m2etis::message::AckDeliverInfo)
BOOST_CLASS_EXPORT(m2etis::message::NackDeliverInfo)
// Validity
BOOST_CLASS_EXPORT(m2etis::message::TimeValidityInfo)

// Spreadit / Null for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Spreadit_Null_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_OMNETMessage, "0")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_OMNETMessage, boost::serialization::object_serializable)

// Spreadit / DetMerge for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::order::DetMergeConfig>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Spreadit_DetMerge_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_DetMerge_OMNETMessage, "1")
BOOST_CLASS_IMPLEMENTATION(Spreadit_DetMerge_OMNETMessage, boost::serialization::object_serializable)

// Spreadit / MTP for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Spreadit_MTP_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_MTP_OMNETMessage, "2")
BOOST_CLASS_IMPLEMENTATION(Spreadit_MTP_OMNETMessage, boost::serialization::object_serializable)

// Spreadit / GMS for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Spreadit_GMS_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_GMS_OMNETMessage, "3")
BOOST_CLASS_IMPLEMENTATION(Spreadit_GMS_OMNETMessage, boost::serialization::object_serializable)

// Direct / Null for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Direct_Null_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_OMNETMessage, "4")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_OMNETMessage, boost::serialization::object_serializable)

// Direct / MTP for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Direct_MTP_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Direct_MTP_OMNETMessage, "5")
BOOST_CLASS_IMPLEMENTATION(Direct_MTP_OMNETMessage, boost::serialization::object_serializable)

// Direct / DetMerge for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::order::DetMergeConfig>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Direct_DetMerge_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Direct_DetMerge_OMNETMessage, "6")
BOOST_CLASS_IMPLEMENTATION(Direct_DetMerge_OMNETMessage, boost::serialization::object_serializable)

// Direct / GMS for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Direct_GMS_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Direct_GMS_OMNETMessage, "7")
BOOST_CLASS_IMPLEMENTATION(Direct_GMS_OMNETMessage, boost::serialization::object_serializable)

// DirectBroadcast / Null for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> DirectBroadcast_Null_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_OMNETMessage, "8")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_OMNETMessage, boost::serialization::object_serializable)

// DirectBroadcast / MTP for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> DirectBroadcast_MTP_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_MTP_OMNETMessage, "9")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_MTP_OMNETMessage, boost::serialization::object_serializable)

// DirectBroadcast / DetMerge for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::order::DetMergeConfig>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> DirectBroadcast_DetMerge_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_DetMerge_OMNETMessage, "10")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_DetMerge_OMNETMessage, boost::serialization::object_serializable)

// DirectBroadcast / GMS for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> DirectBroadcast_GMS_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_GMS_OMNETMessage, "11")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_GMS_OMNETMessage, boost::serialization::object_serializable)

// Scribe / Null for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::ScribeRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::OMNET>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::AckDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>, 5, m2etis::pubsub::deliver::Amount::EXACTLY_ONCE>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Scribe_Null_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Scribe_Null_OMNETMessage, "12")
BOOST_CLASS_IMPLEMENTATION(Scribe_Null_OMNETMessage, boost::serialization::object_serializable)

// Direct / Null / Ack for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::deliver::AckDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>, 5, m2etis::pubsub::deliver::Amount::EXACTLY_ONCE>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Direct_Null_Ack_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Ack_OMNETMessage, "13")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Ack_OMNETMessage, boost::serialization::object_serializable)

// Spreadit / Null for OMNET
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::TimeValidity<0, 5000000>
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Spreadit_Null_Time_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Time_OMNETMessage, "14")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Time_OMNETMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::DirectBroadcastPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Direct_DirectBroadcastPartition_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Direct_DirectBroadcastPartition_OMNETMessage, "15")
BOOST_CLASS_IMPLEMENTATION(Direct_DirectBroadcastPartition_OMNETMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::OMNET>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::filter::NullFilter<SimulationEventType, m2etis::net::NetworkType<m2etis::net::OMNET> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::OMNET>>
, m2etis::pubsub::deliver::NackDeliver<m2etis::net::NetworkType<m2etis::net::OMNET>, 5, m2etis::pubsub::deliver::Amount::EXACTLY_ONCE>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::OMNET>, SimulationEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, SimulationEventType> Direct_Null_Nack_OMNETMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Nack_OMNETMessage, "16")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Nack_OMNETMessage, boost::serialization::object_serializable)

#else // WITH_TESTING case

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


//with char vector instead of position:


//necessary exports to serialize filters
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
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
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
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, "1")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "2")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "3")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "4")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, "5")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "6")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "7")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "8")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, "9")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "10")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "11")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "12")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, "13")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "14")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "15")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "16")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, "17")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "18")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "19")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "20")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, "21")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "22")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "23")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "24")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, "25")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "26")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "27")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "28")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, "29")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "30")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "31")
BOOST_CLASS_IMPLEMENTATION(Direct_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "32")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, "33")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "34")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "35")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "36")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, "37")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "38")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "39")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "40")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, "41")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "42")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "43")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "44")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, "45")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "46")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "47")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "48")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, "49")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "50")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "51")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "52")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, "53")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "54")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "55")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "56")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, "57")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "58")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "59")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "60")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, "61")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "62")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> DirectBroadcast_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(DirectBroadcast_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "63")
BOOST_CLASS_IMPLEMENTATION(DirectBroadcast_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "64")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, "65")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "66")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "67")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "68")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, "69")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "70")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "71")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "72")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, "73")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "74")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "75")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "76")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, "77")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "78")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "79")
BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "80")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, "81")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_Null_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "82")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "83")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_Null_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, "84")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, "85")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_DetMerge_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "86")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::DetMergeOrder<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::config::order::DetMergeConfigTest>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "87")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_DetMerge_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, "88")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, "89")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_MTP_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "90")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::MTPOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000, m2etis::pubsub::order::LateDeliver::DROP>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "91")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_MTP_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, "92")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, "93")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_GMS_Null_Null_Null_Null_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::TCP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, "94")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_TCPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::BruteForceFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::GMSOrder<m2etis::net::NetworkType<m2etis::net::UDP>, 1000000>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::BruteForcePartition<m2etis::net::NetworkType<m2etis::net::UDP>, CharVectorEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, "95")
BOOST_CLASS_IMPLEMENTATION(Spreadit_BruteForce_GMS_Null_Null_Null_BruteForce_Null_CharVector_UDPMessage, boost::serialization::object_serializable)

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::UDP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectRouting<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::filter::NullFilter<IntegrationTestEventType, m2etis::net::NetworkType<m2etis::net::UDP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::UDP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::DirectBroadcastPartition<m2etis::net::NetworkType<m2etis::net::UDP>, IntegrationTestEventType>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, IntegrationTestEventType> Direct_Integration_UDPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Integration_UDPMessage, "96")
BOOST_CLASS_IMPLEMENTATION(Direct_Integration_UDPMessage, boost::serialization::object_serializable)

#endif

#endif
