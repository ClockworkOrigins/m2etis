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

cplusplus {{

#include <OverlayKey.h>
#include <string>

#undef ev
	#include <m2etis/message/M2Message.h>
#define ev  (*cSimulation::getActiveEnvir())
}}
class noncobject OverlayKey;
class noncobject m2etis::message::M2SimMessage::Ptr;

enum MessageType
{
   O_NONE=0;
   O_PUBLISH=1;
   O_SUBSCRIBE=2;
   O_UNSUBSCRIBE=3;
   O_CONTROL=4;
   O_NOTIFY=5;
   O_JOIN=6;
   O_STATE=7;
   O_LEAVE=8;
}

packet SimM2etisMessage {
    //Hack due to lack of c++ enum support!???
    int id;
    int channelName;
    OverlayKey key;
    int type enum(MessageType);
    m2etis::message::M2SimMessage::Ptr intmessage;
 	OverlayKey ownerName; 
}
