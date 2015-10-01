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

/**
 * \addtogroup pubsub
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_PUBSUBSYSTEM_H__
#define __M2ETIS_PUBSUB_PUBSUBSYSTEM_H__

#include "m2etis/util/ExceptionQueue.h"
#include "m2etis/util/SystemParameters.h"

#include "m2etis/message/M2Message.h"

#include "m2etis/pubsub/ChannelEventInterface.h"
#include "m2etis/pubsub/ChannelInterface.h"
#include "m2etis/pubsub/config/ChannelConfiguration.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"

#include "boost/function.hpp"

// Forward Declarations

namespace m2etis {
namespace pubsub {

	template <class EventType> class BasicDeliverCallbackInterface;
	class PubSubSystemEnvironment;

	enum exceptionEvents {
		CONNECTION_FAILED = 0,
		CONNECTION_CLOSED = 1,
		NODEFAILURE = 2,
		UNDEFINED = 3,
		BINDFAILURE = 4
	};

	/**
	 * \ingroup pubsub
	 *
	 * \class PubSubSystem
	 * \brief class for accessing the m2etis pub/sub system. It is the main entry point for the usage of the m2etis networking library.
	 *
	 * Long Desc
	 *
	 */
	class M2ETIS_API PubSubSystem {
	public:
		PubSubSystem(const std::string & listenIP, const uint16_t listenPort, const std::string & connectIP, const uint16_t connectPort, const std::vector<std::string> & rootList);

		~PubSubSystem();

		/**
		 * \brief User subscribes to the requested channel
		 * \param[in] channel requested channel
		 * \param[in, out] callback callback method, which the publisher can call
		 * \return Returns a Handle to interact with the requested channel
		 */
		template <class EventType> BasicChannelInterface<EventType> & subscribe(const ChannelName channel, BasicDeliverCallbackInterface<EventType> & callback);

		// FIXME create predicate interface
		/**
		 * \brief User subscribes to the requested channel
		 * \param[in] channel requested channel
		 * \param[in] callback callback method, which the publisher can call
		 * \param[in] predicate filter
		 * \return Returns a Handle to interact with the requested channel
		 */
		template <class EventType> BasicChannelInterface<EventType> & subscribe(const ChannelName channel, BasicDeliverCallbackInterface<EventType> & callback, boost::shared_ptr<filter::FilterExp<EventType> > predicate);

		/**
		 * \brief User unsubscribes to the requested channel
		 * \param[in] channel channel
		 * \return Returns a pointer to the PubSubSystem
		 */
		template <class EventType> const PubSubSystem & unsubscribe(const ChannelName channel) const;

		/**
		 * \brief User deregisters single filter from the requested channel
		 * \param[in] channel channel
		 * \param[in] predicate filter
		 * \return Returns a pointer to the PubSubSystem
		 */
		template <class EventType> const PubSubSystem & unsubscribe(const ChannelName channel, const boost::shared_ptr<filter::FilterExp<EventType> > predicate) const;

		/**
		 * \brief Publishes a message to a channel
		 * \param[in] channel channel
		 * \param[in] publish_message published message
		 * \return Returns a pointer to the PubSubSystem
		 */
		template <class EventType> const PubSubSystem & publish(const ChannelName channel, const typename message::M2Message<EventType>::Ptr publish_message) const;

		/**
		 * \brief returns a M2Message for the given EventType and Payload
		 */
		// FIXME: (Daniel) if we kick M2Message, we can kick also these to methods and publish Payload directly
		template <class EventType> typename message::M2Message<EventType>::Ptr createMessage(const ChannelName channel, const EventType & payload);
		template <class EventType> typename message::M2Message<EventType>::Ptr createMessage(const ChannelName channel);

		/**
		 * \brief Returns a string representation of the requested channel
		 * \param[in] channel channel
		 * \return string representation of the requested channel
		 */
		template <class EventType> std::string getSelf(const ChannelName channel) const;

		/**
		 * \brief sets callback for disconnect
		 */
		inline bool isInitialized() const { return initialized; }

		void registerExceptionCallback(exceptionEvents e, boost::function<void(const std::string &)> _ptr);

	private:
		PubSubSystem(const PubSubSystem &) = delete;
		PubSubSystem & operator=(const PubSubSystem &) = delete;

		bool exceptionLoop();

	public:
		PubSubSystemEnvironment * _pssi;

	private:
		ChannelConfiguration * channels_;
		bool initialized;

		std::vector<std::vector<boost::function<void(const std::string &)>>> _exceptionCallbacks;

		uint64_t exceptionID_;

		bool _running;

#ifdef WITH_SIM
	public:
#endif
		/**
		 * \brief Gets a Handle to interact with the requested channel
		 *
		 * \param[in] channel identifies the channel as previously defined by the ChannelName enum.
		 * @see ChannelName
		 * \return Returns a Handle to interact with the requested channel
		 */
		template<class EventType> BasicChannelInterface<EventType> * getChannelHandle(const ChannelName channel) const;
	};

	template <class EventType> std::string PubSubSystem::getSelf(ChannelName channel) const {
		return "";
	}

	template <class EventType> BasicChannelInterface<EventType> * PubSubSystem::getChannelHandle(ChannelName channel) const {
		if (!initialized) {
			M2ETIS_THROW_API("PubSubSystem", "Invalid call, initialize PubSubSystem first.");
		}
		if (channel >= channels_->count) {
			M2ETIS_THROW_API("PubSubSystem", std::string("Invalid channel, enum exceeds CHANNEL_COUNT: ") + boost::lexical_cast<std::string>(channel));
		}
		BasicChannelInterface<EventType> * const ret = dynamic_cast<BasicChannelInterface<EventType> * const>(channels_->channels()[channel]);
		if (ret == NULL) {
			M2ETIS_THROW_API("PubSubSystem", "Invalid channel type cast. Check your channel configuration.");
		}
		return ret;
	}

	template <class EventType> BasicChannelInterface<EventType> & PubSubSystem::subscribe(ChannelName channel, BasicDeliverCallbackInterface<EventType> & callback) {
		const boost::shared_ptr<filter::FilterExp<EventType> > pred = boost::make_shared<filter::TruePredicate<EventType> >();

		return subscribe(channel, callback, pred);
	}

	template <class EventType> BasicChannelInterface<EventType> & PubSubSystem::subscribe(const ChannelName channel, BasicDeliverCallbackInterface<EventType> & callback, const boost::shared_ptr<filter::FilterExp<EventType> > predicate) {
		BasicChannelInterface<EventType> * const ret = getChannelHandle<EventType>(channel);

		ret->subscribe(callback, predicate);
		return *ret;
	}

	template <class EventType> const PubSubSystem & PubSubSystem::unsubscribe(ChannelName channel) const {
		BasicChannelInterface<EventType> * const ret = getChannelHandle<EventType>(channel);
		ret->unsubscribe();
		return *this;
	}


	// for filter strategies: deregistering single filter:
	template <class EventType> const PubSubSystem & PubSubSystem::unsubscribe(const ChannelName channel, const boost::shared_ptr<filter::FilterExp<EventType> > predicate) const {
		BasicChannelInterface<EventType> * const ret = getChannelHandle<EventType>(channel);
		ret->unsubscribe(predicate);
		return *this;
	}

	template <class EventType> const PubSubSystem & PubSubSystem::publish(const ChannelName channel, const typename message::M2Message<EventType>::Ptr publish_message) const {
		BasicChannelInterface<EventType> * const ret = getChannelHandle<EventType>(channel);
		ret->publish(publish_message);
		return *this;
	}

	template <class EventType> typename message::M2Message<EventType>::Ptr PubSubSystem::createMessage(const ChannelName channel, const EventType & payload) {
		return getChannelHandle<EventType>(channel)->createMessage(payload);
	}

	template <class EventType> typename message::M2Message<EventType>::Ptr PubSubSystem::createMessage(const ChannelName channel) {
		return createMessage<EventType>(channel, EventType());
	}

} /* namespace pubsub */
} /* namespace m2etis */

/**
 *  @}
 */

#endif /* __M2ETIS_PUBSUB_PUBSUBSYSTEM_H__ */
