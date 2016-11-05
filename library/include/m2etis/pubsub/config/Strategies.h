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
 * \addtogroup pubsub
 * @{
 */

#ifndef __M2ETIS_STRATEGIES_H__
#define __M2ETIS_STRATEGIES_H__

#include <exception>

#include "m2etis/message/key/Key.h"
#include "m2etis/message/key/providers/IPv4KeyProvider.h"
#include "m2etis/message/key/providers/SHA1KeyProvider.h"

#include "m2etis/pubsub/routing/DirectRouting.h"
#include "m2etis/pubsub/routing/DirectBroadcastRouting.h"
#include "m2etis/pubsub/routing/HierarchicalSpreaditRouting.h"
#include "m2etis/pubsub/routing/SpreaditRouting.h"
#include "m2etis/pubsub/routing/ScribeRouting.h"

#include "m2etis/pubsub/filter/NullFilter.h"
#include "m2etis/pubsub/filter/BruteForceFilter.h"
#include "m2etis/pubsub/filter/DecisionTreeFilter.h"
#include "m2etis/pubsub/filter/GeneralBooleanExpressionsFilter.h"

#include "m2etis/pubsub/persistence/NullPersistence.h"

#include "m2etis/pubsub/validity/NullValidity.h"
#include "m2etis/pubsub/validity/TimeValidity.h"

#include "m2etis/pubsub/security/NullSecurity.h"

#include "m2etis/pubsub/order/NullOrder.h"
#include "m2etis/pubsub/order/MTPOrder.h"
#include "m2etis/pubsub/order/GMSOrder.h"
#include "m2etis/pubsub/order/DetMergeOrder.h"
#include "m2etis/pubsub/order/DetMergeConfig.h"

#include "m2etis/pubsub/deliver/NullDeliver.h"
#include "m2etis/pubsub/deliver/AckDeliver.h"
#include "m2etis/pubsub/deliver/NackDeliver.h"

#include "m2etis/pubsub/partition/NullPartition.h"
#include "m2etis/pubsub/partition/BruteForcePartition.h"
#include "m2etis/pubsub/partition/DirectBroadcastPartition.h"

#include "m2etis/pubsub/rendezvous/NullRendezvous.h"

#endif /* __M2ETIS_STRATEGIES_H__ */

/**
 * @}
 */
