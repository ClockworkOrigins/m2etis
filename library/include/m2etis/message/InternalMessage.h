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

/**
 * \addtogroup message
 * @ {
 */

#ifndef __M2ETIS_MESSAGE_INTERNALMESSAGE_H__
#define __M2ETIS_MESSAGE_INTERNALMESSAGE_H__

#include "m2etis/message/M2Message.h"
#include "m2etis/message/NetworkMessage.h"

#include "m2etis/message/info/DeliverInfo.h"
#include "m2etis/message/info/FilterInfo.h"
#include "m2etis/message/info/OrderInfo.h"
#include "m2etis/message/info/PersistenceInfo.h"
#include "m2etis/message/info/RoutingInfo.h"
#include "m2etis/message/info/SecurityInfo.h"
#include "m2etis/message/info/ValidityInfo.h"

#include "boost/serialization/export.hpp"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/variant.hpp"
#include "boost/serialization/vector.hpp"

namespace m2etis {
namespace message {

	enum class ControlType : short {
		NONE = 0,
		DELIVER,
		FILTER,
		ORDER,
		PERSISTENCE,
		ROUTING,
		SECURITY,
		VALIDITY
	};

	template<class NetworkType, class ChannelType, class EventType>
	class InternalMessage : public M2Message<EventType>, public NetworkMessage<NetworkType> {
	public:
		// Message Ptr
		typedef boost::shared_ptr<InternalMessage> Ptr;

		typedef typename ChannelType::DeliverStrategy::DeliverInfoType DInfo;
		typedef typename ChannelType::FilterStrategy::FilterInfoType FInfo;
		typedef typename ChannelType::OrderStrategy::OrderInfoType OInfo;
		typedef typename ChannelType::PersistenceStrategy::PersistenceInfoType PInfo;
		typedef typename ChannelType::RoutingStrategy::RoutingInfoType RInfo;
		typedef typename ChannelType::SecurityStrategy::SecurityInfoType SInfo;
		typedef typename ChannelType::ValidityStrategy::ValidityInfoType VInfo;

		typedef struct TreeHelper {
			uint16_t topic;
			boost::shared_ptr<pubsub::filter::FilterExp<EventType>> predicates;
			typename NetworkType::Key root;

			TreeHelper() : topic(), predicates(), root() {}
			TreeHelper(uint16_t t, boost::shared_ptr<pubsub::filter::FilterExp<EventType>> p, typename NetworkType::Key r) : topic(t), predicates(p), root(r) {
			}

	        friend class boost::serialization::access;
	        template <class Archive>
			void serialize(Archive & ar, const unsigned int) {
	        	ar & topic;
	        	ar & predicates;
	        	ar & root;
			}
		} TreeHelper;

	    MessageType type;

		/**
		 * \brief contains message dependent datas for Delivering
		 */
		typename DInfo::Ptr deliverInfo;

		/**
		 * \brief contains message dependent datas for Filtering
		 */
		// FilterPtr filterInfo;
        typename FInfo::Ptr filterInfo;

		/**
		 * \brief contains message dependent datas for Odering
		 */
		typename OInfo::Ptr orderInfo;

		/**
		 * \brief contains message dependent datas for Persistency
		 */
		typename PInfo::Ptr persistenceInfo;

        /**
		 * \brief contains message dependent datas for Routing
		 */
        typename RInfo::Ptr routingInfo;

		/**
		 * \brief contains message dependent datas for Security
		 */
		typename SInfo::Ptr securityInfo;

		/**
		 * \brief contains message dependent datas for Validity
		 */
		typename VInfo::Ptr validityInfo;

		/**
		 * \brief type containing the strategie that sent this message
		 */
		ControlType ctrlType_;

		/**
		 * \brief list of all nodes for join
		 */
		std::vector<typename NetworkType::Key> _nodeList;

		/**
		 * \brief current time of RP
		 */
		uint64_t _time;

		/**
		 * \brief list of all trees existing
		 */
		std::vector<TreeHelper> _trees;

		/**
		 * \brief list of all topics being removed during leave
		 */
		std::set<uint16_t> _topics;

		InternalMessage() : M2Message<EventType>(&type), NetworkMessage<NetworkType>(&type),
			type(),
			deliverInfo(boost::make_shared<DInfo>()),
			filterInfo(boost::make_shared<FInfo>()),
			orderInfo(boost::make_shared<OInfo>()),
			persistenceInfo(boost::make_shared<PInfo>()),
			routingInfo(boost::make_shared<RInfo>()),
			securityInfo(boost::make_shared<SInfo>()),
			validityInfo(boost::make_shared<VInfo>()),
			ctrlType_(ControlType::NONE),
			_nodeList(),
			_time(0),
			_trees(),
			_topics() {
		}

		explicit InternalMessage(const EventType & v) : M2Message<EventType>(v, &type), NetworkMessage<NetworkType>(&type),
			type(),
			deliverInfo(boost::make_shared<DInfo>()),
			filterInfo(boost::make_shared<FInfo>()),
			orderInfo(boost::make_shared<OInfo>()),
			persistenceInfo(boost::make_shared<PInfo>()),
			routingInfo(boost::make_shared<RInfo>()),
			securityInfo(boost::make_shared<SInfo>()),
			validityInfo(boost::make_shared<VInfo>()),
			ctrlType_(ControlType::NONE),
			_nodeList(),
			_time(0),
			_trees(),
			_topics() {
		}

		explicit InternalMessage(const InternalMessage & msg) : M2Message<EventType>(msg, &type), NetworkMessage<NetworkType>(msg, &type),
			type(msg.type),
			deliverInfo(boost::make_shared<DInfo>(*msg.deliverInfo)),
			filterInfo(boost::make_shared<FInfo>(*msg.filterInfo)),
			orderInfo(boost::make_shared<OInfo>(*msg.orderInfo)),
			persistenceInfo(boost::make_shared<PInfo>(*msg.persistenceInfo)),
			routingInfo(boost::make_shared<RInfo>(*msg.routingInfo)),
			securityInfo(boost::make_shared<SInfo>(*msg.securityInfo)),
			validityInfo(boost::make_shared<VInfo>(*msg.validityInfo)),
			ctrlType_(msg.ctrlType_),
			_nodeList(msg._nodeList),
			_time(msg._time),
			_trees(msg._trees),
			_topics(msg._topics) {
		}

		~InternalMessage() {
		}

		std::string toString() {
			std::stringstream ss;
			ss << "IMessage(" << int(type) << ", " << int(ctrlType_) << ", removeTopics: " << _topics.size() << ", trees: " << _trees.size() << "(";
			for (int i = 0; i < _trees.size(); i++) {
				ss << _trees[i].topic << ":" << _trees[i].root.toStr() << ", ";
			}
			ss << "), nodelist: " << _nodeList.size() << "(";
			for (size_t i = 0; i < _nodeList.size(); i++) {
				ss << _nodeList[i].toStr() << ", ";
			}
			ss << ")";
			ss << ")";
			return ss.str();
		}
	private:
        friend class boost::serialization::access;

        template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
        	ar & type;
        	ActionType actionType = ActionType(type & ACTION_TYPE_MASK);
        	if (actionType == PUBLISH || actionType == NOTIFY) {
        		ar & boost::serialization::base_object<m2etis::message::M2Message<EventType>>(*this);
        	}
        	ar & boost::serialization::base_object<m2etis::message::NetworkMessage<NetworkType>>(*this);
        	if (RInfo::doSerialize(actionType)) {
        		ar & routingInfo;
        	}
			if (FInfo::doSerialize(actionType)) {
				ar & filterInfo;
			}
			if (PInfo::doSerialize(actionType)) {
				ar & persistenceInfo;
			}
			if (VInfo::doSerialize(actionType)) {
				ar & validityInfo;
			}
			if (SInfo::doSerialize(actionType)) {
				ar & securityInfo;
			}
			if (OInfo::doSerialize(actionType)) {
				ar & orderInfo;
			}
			if (DInfo::doSerialize(actionType)) {
				ar & deliverInfo;
			}
			if (actionType == CONTROL) {
				ar & ctrlType_;
			}
			if (actionType == STATE) {
				ar & _nodeList;
				ar & _time;
				ar & _trees;
			}
			if (actionType == LEAVE) {
				ar & _topics;
			}
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_INTERNALMESSAGE_H__ */

/**
 *  @}
 */
