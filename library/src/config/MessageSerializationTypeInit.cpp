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

/*
 * This file generates the explicit type instantiations for the derived message types. These are used by boost serialization to work on the messages polymorphic.
 * It uses the channel configuration in order to derive the types for messages.
 */

#include "m2etis/message/MessageSerialization.h"
#include "m2etis/pubsub/config/Strategies.h"

#include "m2etis/config/GeneratedMessageConfiguration.h"

// RoutingInfo derived types
