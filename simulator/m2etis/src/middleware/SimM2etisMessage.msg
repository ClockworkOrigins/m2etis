//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

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
