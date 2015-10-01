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
 * \addtogroup simulator
 * @ {
 */

#ifndef __M2ETIS_APPLICATIONCALLBACK_H__
#define __M2ETIS_APPLICATIONCALLBACK_H__

#undef ev
	#include "m2etis/config/GeneratedEventTypes.h"
	#include "m2etis/pubsub/DeliverCallbackInterface.h"
#define ev  (*cSimulation::getActiveEnvir())

#include "middleware/SimM2etisMessage_m.h"

#include "BaseApp.h"

namespace m2etis {

	class ApplicationCallback : public pubsub::BasicDeliverCallbackInterface<SimulationEventType> {
	public:
		/**
		 * \brief constructor taking the M2etisAdapter it belongs to
		 *
		 * \params[in] the M2etisAdapter this callback belongs to
		 */
		explicit ApplicationCallback(BaseApp * adapter) : adapter_(adapter) {}

		/**
		 * \brief callback method used in m2etis to deliver messages to simulator
		 *
		 * \param[in] msg the message being delivered by m2etis to the simulator
		 */
		void deliverCallback(const message::M2SimMessage::Ptr msg);

		/**
		 * \brief destructor
		 */
		~ApplicationCallback();

	private:
		BaseApp * adapter_;
	};

} /* namespace m2etis */

#endif /* __M2ETIS_APPLICATIONCALLBACK_H__ */

/**
 *  @}
 */
