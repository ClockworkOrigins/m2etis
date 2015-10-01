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

#include <string>

#undef ev
	#include "m2etis/sim/i6eRandom.h"
#define ev  (*cSimulation::getActiveEnvir())

#include "UnderlayConfigurator.h"
#include "GlobalStatistics.h"

#include "TupleFeeder.h"

#include <middleware/SimM2etisMessage_m.h>

namespace m2etis {

	Define_Module(TupleFeeder);

	int TupleFeeder::numSubs = 0;
	int TupleFeeder::numPubs = 0;
	int TupleFeeder::numPubSubs = 0;
	int TupleFeeder::numRend = 0;
	int TupleFeeder::numPubsR = 0;

	bool TupleFeeder::rootNodeInitialized = false;

	int TupleFeeder::messageID = 1;

	double TupleFeeder::minMsgTime = -1;
	double TupleFeeder::maxMsgTime = -1;
	double TupleFeeder::allTimes = 0;
	int TupleFeeder::msgCounter = 0;
	double TupleFeeder::msgAvgTime = 0;

	double TupleFeeder::minMsgCTime = -1;
	double TupleFeeder::maxMsgCTime = -1;
	double TupleFeeder::allCTimes = 0;
	int TupleFeeder::msgCCounter = 0;
	double TupleFeeder::msgCAvgTime = 0;

	double TupleFeeder::minFCTime = -1;
	double TupleFeeder::maxFCTime = -1;
	double TupleFeeder::allFCTimes = 0;
	int TupleFeeder::FCCounter = 0;
	double TupleFeeder::FCAvgTime = 0;

	int TupleFeeder::duplicateMsgs = 0;

	std::map<int, TupleFeeder::AliveMessage > TupleFeeder::_messages = std::map<int, TupleFeeder::AliveMessage >();

	int TupleFeeder::_channelCount = 0;
	std::map<int, ChannelInfoStruct *> TupleFeeder::_channels;

	double TupleFeeder::lossTimeout = 0;

	boost::posix_time::ptime TupleFeeder::startTime = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::ptime TupleFeeder::endTime = boost::posix_time::microsec_clock::universal_time();

	int TupleFeeder::id = 0;

	bool TupleFeeder::stopAll = false;

	double TupleFeeder::minStayDuration = 1.0;

	double TupleFeeder::startTimeNode = -1.0;

	void TupleFeeder::initializeApp(int stage) {
		if (stage == MIN_STAGE_APP) {
			// copy the module parameter values to our own variables
			largestKey = par("largestKey");
			_stopAvg = par("stopAvg");
			_stopLoss = par("stopLoss");
			_waitForSubscribe = par("waitForSubscribe");
			_waitForPublish = par("waitForPublish");
			minStayDuration = par("minStayDuration");

			if (_channelCount == 0) {
				numRend = par("numRend");
				numSubs = par("numSubs");
				numPubs = par("numPubs");
				numPubSubs = par("numPubSubs");

				_channelCount = par("channelCount");

				startTime = boost::posix_time::microsec_clock::universal_time();

				for (int i = 0; i < _channelCount; ++i) {
					ChannelInfoStruct * cis = new ChannelInfoStruct();

					cis->channel = i;

					cis->numToSend = par((std::string("numToSend_") + boost::lexical_cast<std::string>(i)).c_str());

					cis->burstAmount = par((std::string("burstAmount_") + boost::lexical_cast<std::string>(i)).c_str());
					cis->burstFrequency = par((std::string("burstFrequency_") + boost::lexical_cast<std::string>(i)).c_str());
					cis->burstDuration = par((std::string("burstDuration_") + boost::lexical_cast<std::string>(i)).c_str());

					cis->burstFrequency = cis->burstFrequency / sendPeriod;
					cis->burstDuration = cis->burstDuration / sendPeriod;

					cis->dropChance = par((std::string("dropChance_") + boost::lexical_cast<std::string>(i)).c_str());
					cis->dropChance *= 100.0;
					cis->compensateDrop = par((std::string("compensateDrop_") + boost::lexical_cast<std::string>(i)).c_str());

					double flucci = par((std::string("fluctuation_") + boost::lexical_cast<std::string>(i)).c_str());

					cis->fluctuation = flucci * (numSubs + numPubSubs);
					cis->fluctuationID = cis->fluctuation;

					if (cis->numToSend == 0) { // Channels not doing anything mustn't be stored
						delete cis;

						continue;
					}

					_channels[i] = cis;

					WATCH(cis->minMsgTime);
					WATCH(cis->maxMsgTime);
					WATCH(cis->msgAvgTime);

					WATCH(cis->minMsgCTime);
					WATCH(cis->maxMsgCTime);
					WATCH(cis->msgCAvgTime);

					WATCH(cis->minFCTime);
					WATCH(cis->maxFCTime);
					WATCH(cis->FCAvgTime);

					WATCH(cis->duplicateMsgs);

					WATCH(cis->isOrdered);

					_burst[i] = false;
				}
			}

			for (std::pair<int, ChannelInfoStruct *> p : _channels) {
				sendStruct * s = new sendStruct();
				s->sendPeriod = 1.0 / p.second->numToSend;

				if (s->sendPeriod > 1.0) {
					s->ticksPerSec = 1.0 / p.second->numToSend;
					s->sendPeriod = 1.0;
				} else {
					s->ticksPerSec = p.second->numToSend;
				}

				_sendChannels[p.first] = s;
			}

			// initialize our statistics variables
			numSent = 0;
			numReceived = 0;

			// tell the GUI to display our variables
			WATCH(numSent);
			WATCH(numReceived);

			WATCH(minMsgTime);
			WATCH(maxMsgTime);
			WATCH(msgAvgTime);

			WATCH(minMsgCTime);
			WATCH(maxMsgCTime);
			WATCH(msgCAvgTime);

			WATCH(minFCTime);
			WATCH(maxFCTime);
			WATCH(FCAvgTime);

			WATCH(duplicateMsgs);
		}
	}

	void TupleFeeder::finishApp() {
		// finish() is usually used to save the module's statistics.
		// We'll use globalStatistics->addStdDev(), which will calculate min, max, mean and deviation values.
		// The first parameter is a name for the value, you can use any name you like (use a name you can find quickly!).
		// In the end, the simulator will mix together all values, from all nodes, with the same name.

		globalStatistics->addStdDev("TupleFeeder: Sent packets", numSent);
		globalStatistics->addStdDev("TupleFeeder: Received packets", numReceived);

		globalStatistics->addStdDev("TupleFeeder: Latency - One node - Min", minMsgTime);
		globalStatistics->addStdDev("TupleFeeder: Latency - One node - Max", maxMsgTime);
		globalStatistics->addStdDev("TupleFeeder: Latency - One node - Avg", msgAvgTime);

		globalStatistics->addStdDev("TupleFeeder: Latency - All nodes - Min", minMsgCTime);
		globalStatistics->addStdDev("TupleFeeder: Latency - All nodes - Max", maxMsgCTime);
		globalStatistics->addStdDev("TupleFeeder: Latency - All nodes - Avg", msgCAvgTime);

		globalStatistics->addStdDev("TupleFeeder: First contact - Min", minFCTime);
		globalStatistics->addStdDev("TupleFeeder: First contact - Max", maxFCTime);
		globalStatistics->addStdDev("TupleFeeder: First contact - Avg", FCAvgTime);

		globalStatistics->addStdDev("TupleFeeder: Duplicated messages at receiver", duplicateMsgs);

		globalStatistics->addStdDev("TupleFeeder: Time", static_cast<long>(boost::posix_time::time_period(startTime, boost::posix_time::microsec_clock::universal_time()).length().total_microseconds())/1000000.0);

		for (std::pair<int, ChannelInfoStruct *> p : _channels) {
			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Latency - One node - Min"), p.second->minMsgTime);
			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Latency - One node - Max"), p.second->maxMsgTime);
			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Latency - One node - Avg"), p.second->msgAvgTime);

			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Latency - All nodes - Min"), p.second->minMsgCTime);
			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Latency - All nodes - Max"), p.second->maxMsgCTime);
			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Latency - All nodes - Avg"), p.second->msgCAvgTime);

			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: First contact - Min"), p.second->minFCTime);
			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: First contact - Max"), p.second->maxFCTime);
			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: First contact - Avg"), p.second->FCAvgTime);

			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Duplicated messages at receiver"), p.second->duplicateMsgs);

			globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Channel ordered"), p.second->isOrdered);

			if ((_state == SUBSCRIBER || _state == PUBSUBER) && _orderMap[p.first]._overAll > 0) {
				globalStatistics->addStdDev(std::string("TupleFeeder <Channel ") + boost::lexical_cast<std::string>(p.first) + std::string(">: Channel ordered percentage"), boost::lexical_cast<float>(_orderMap[p.first]._inOrder) * 100.0f / _orderMap[p.first]._overAll);
			}
		}

		assert(_initialized);
		for (std::pair<int, sendStruct *> s : _sendChannels) {
			//cancelAndDelete(s.second->timerMsg); // TODO: (Daniel) hack because simulation crashs used by Michi and Andi
		}

		globalStatistics->addStdDev("TupleFeeder: Stopped", _stopped);

		if (minMsgTime == -1 || minMsgCTime == -1) {
			globalStatistics->addStdDev("TupleFeeder: Success", false);
		} else {
			globalStatistics->addStdDev("TupleFeeder: Success", true);
		}

		double perc = getCurrentLoss();

		perc *= 100.0;

		if (_stopped) {
			globalStatistics->addStdDev("TupleFeeder: Message Lost nodes percent", _stopLoss * 100.0);
		} else {
			globalStatistics->addStdDev("TupleFeeder: Message Lost nodes percent", perc);
		}

		if (_runTime == -1.0) {
			globalStatistics->addStdDev("TupleFeeder: Simulated seconds", simTime().dbl() - startTimeNode);
		} else {
			globalStatistics->addStdDev("TupleFeeder: Simulated seconds", _runTime);
		}
	}

	void TupleFeeder::handleTimerEvent(cMessage * msg) {
		// start our timer!

		if (msg == waitForInitMsg && startTimeNode == -1.0) {
			scheduleAt(simTime() + 0.1, msg);
			return;
		} else if (msg == waitForInitMsg && startTimeNode != -1.0) {
			_subscribeMsg = new cMessage("TupleFeeder Timer");
			scheduleAt(simTime() + _waitForSubscribe, _subscribeMsg);
			delete msg;
			return;
		}

		if (!_initialized) {
			for (std::pair<int, sendStruct *> p : _sendChannels) {
				p.second->timerMsg = new cMessage("TupleFeeder Timer");
				scheduleAt(simTime() + _waitForPublish, p.second->timerMsg);
			}

			if (!subscribed_ && (_state == SUBSCRIBER || _state == PUBSUBER)) {
				for (std::pair<int, ChannelInfoStruct *> p : _channels) {
					if (_id >= p.second->fluctuation) {
						SimM2etisMessage * sub = new SimM2etisMessage("OTHER: Subscribe Message");

						_subscribedOnChannel[p.first] = true;

						sub->setType(O_SUBSCRIBE);
						sub->setChannelName(p.first);
						message::M2SimMessage::Ptr m;
						sub->setIntmessage(m);
						sendMessageToLowerTier(sub); // send it to m2etis
						_startSub[p.first] = -1;

						p.second->numSubs++;
					}
				}

				_joinTime = simTime().dbl();

				numPubsR++;

				rootNodeInitialized = true;

				subscribed_ = true;
			}

			_initialized = true;

			delete msg;

			return;
		}


		if (simTime().dbl() - startTimeNode > 10.0) {
			if (msgCAvgTime >= _stopAvg || getCurrentLoss() >= _stopLoss) {
				for (std::pair<int, sendStruct *> s : _sendChannels) {
					cancelAndDelete(s.second->timerMsg);
					delete s.second;
				}

				_sendChannels.clear();

				_stopped = true;
				stopAll = true;

				msgCAvgTime = _stopAvg;

				_runTime = simTime().dbl() - startTimeNode;

				return;
			}
		}

		for (std::pair<int, sendStruct *> s : _sendChannels) {
			if (msg == s.second->timerMsg) {
				// reschedule our message

				// if the simulator is still busy creating the network,
				// let's wait a bit longer
				if (underlayConfigurator->isInInitPhase()) {
					scheduleAt(simTime() + 1, s.second->timerMsg);
					return;
				}

				bool fluctuated = false;

				scheduleAt(simTime() + s.second->sendPeriod, s.second->timerMsg);

				if (_id == 0) {
					_channels[s.first]->missingFluctuation = _channels[s.first]->fluctuation;
				}

				bool isSecond = false;

				if (s.second->sendPeriod < 1.0) {
					if (s.second->ticks % static_cast<int>(s.second->ticksPerSec) == 0) {
						isSecond = true;
					}
				} else {
					isSecond = true;
				}

				// Subscribing
				fluctuated = handleSubscribe(isSecond, s);

				// Unsubscribing
				if (_id == _channels[s.first]->fluctuationID && _state == PUBLISHER) { // skip nodes only publishing
					_channels[s.first]->fluctuationID = (_channels[s.first]->fluctuationID + 1) % id;
				}

				if (isSecond && _subscribedOnChannel[s.first] && _id == _channels[s.first]->fluctuationID && _channels[s.first]->fluctuation > 0 && _channels[s.first]->missingFluctuation > 0 && !fluctuated) {
					_channels[s.first]->fluctuationID = (_channels[s.first]->fluctuationID + 1) % id;
					if ((simTime().dbl() - _joinTime) >= minStayDuration) {
						_channels[s.first]->missingFluctuation--;

						_channels[s.first]->numSubs--;
						SimM2etisMessage * unsub = new SimM2etisMessage("OTHER: Unsubscribe Message");

						unsub->setType(O_UNSUBSCRIBE);
						unsub->setChannelName(s.first);
						message::M2SimMessage::Ptr m;
						unsub->setIntmessage(m);
						sendMessageToLowerTier(unsub); // send it to m2etis

						_subscribedOnChannel[s.first] = false;
					}
				}

				// Publishing
				if (_state == PUBLISHER || _state == PUBSUBER) {
					if (s.second->sendPeriod < 1.0 || (s.second->sendPeriod == 1.0 && s.second->secTicks % static_cast<int>(s.second->ticksPerSec) == 0)) {
						if (isSecond && _channels[s.first]->burstFrequency > 0 && !_burst[s.first] && s.second->secTicks % _channels[s.first]->burstFrequency == 0) {
							_burst[s.first] = true;
						} else if (isSecond && _burst[s.first] && _ticks % (_channels[s.first]->burstFrequency + _channels[s.first]->burstDuration) == 0) {
							_burst[s.first] = false;
						}

						int sendNum = 1;

						if (_burst[s.first] == true) {
							sendNum = _channels[s.first]->burstAmount;
						}

						for (int j = 0; j < sendNum; ++j) {
							if (_channels[s.first]->numSubs == 0) {
								break;
							}
							assert(_channels[s.first]->numSubs > 0);
							_messages[messageID] = {messageID, simTime(), _channels[s.first]->numSubs, _channels[s.first]->numSubs};
							SimM2etisMessage * publish = new SimM2etisMessage("OTHER: Publish Message");
							publish->setId(messageID++);
							publish->setType(O_PUBLISH);
							publish->setChannelName(s.first);
							message::M2SimMessage::Ptr m;
							publish->setIntmessage(m);

							numSent++; // update statistics
							EV << thisNode.getIp() << ": Sending " << publish->getType() << " packet to channel" << publish->getChannelName()
									<< "!" << std::endl;

							sendMessageToLowerTier(publish); // send it to m2etis
						}
					}
				}

				++s.second->ticks;

				if (isSecond) {
					++s.second->secTicks;
				}
			}
		}
	}

	bool TupleFeeder::handleSubscribe(bool sec, std::pair<int, sendStruct *> s) {
		if (sec && !_subscribedOnChannel[s.first] && (_state == SUBSCRIBER || _state == PUBSUBER) && _channels[s.first]->fluctuation > 0) {
			_channels[s.first]->numSubs++;
			SimM2etisMessage * sub = new SimM2etisMessage("OTHER: Subscribe Message");

			sub->setType(O_SUBSCRIBE);
			sub->setChannelName(s.first);
			message::M2SimMessage::Ptr m;
			sub->setIntmessage(m);
			sendMessageToLowerTier(sub); // send it to m2etis

			_subscribedOnChannel[s.first] = true;

			_startSub[s.first] = simTime();

			_joinTime = simTime().dbl();

			return true;
		}

		return false;
	}

	void TupleFeeder::handleLowerMessage(cMessage * msg) {
		SimM2etisMessage * myMsg = dynamic_cast<SimM2etisMessage *>(msg);

		if (myMsg == NULL) {
			delete msg; // type unknown!
			return;
		}

		int cn = myMsg->getIntmessage()->payload->_simChannel;
		int id = myMsg->getIntmessage()->payload->_simID;

		// Message received

		std::map<int, AliveMessage >::iterator it = _messages.find(id);

		simtime_t oldtime;

		if (it != _messages.end()) {
			oldtime = it->second.sendTime;
		}

		simtime_t newtime = simTime();

		simtime_t diff = newtime - oldtime;

		double diff2 = diff.dbl();

		ChannelInfoStruct * cis = _channels[cn];

		if (simTime().dbl() - startTimeNode > 10.0) {
			if (msgCAvgTime >= _stopAvg) {
				for (std::pair<int, sendStruct *> s : _sendChannels) {
					cancelAndDelete(s.second->timerMsg);
					delete s.second;
				}

				_sendChannels.clear();

				_stopped = true;
				stopAll = true;

				msgCAvgTime = _stopAvg;

				delete myMsg;

				return;
			}
		}

		// Message received by all subscribers

		if (!_receivedMsgs.contains(id)) {
			if (it != _messages.end()) {
				assert(it->second.missingRecvs > 0);
				it->second.missingRecvs--;

				_orderMap[cn]._overAll += 1;

				if (_lastIDs.find(cn) == _lastIDs.end()) {
					_lastIDs[cn] = id;
					_orderMap[cn]._inOrder = 1;
				} else {
					if (_lastIDs[cn] > id) {
						cis->isOrdered = false;
						_orderMap[cn]._outOrder += 1;
					} else {
						_orderMap[cn]._inOrder += 1;
					}
				}

				if (diff2 < minMsgTime || minMsgTime == -1) {
					minMsgTime = diff2;
				}

				if (diff2 < cis->minMsgTime || cis->minMsgTime == -1) {
					cis->minMsgTime = diff2;
				}

				if (diff2 > maxMsgTime) {
					maxMsgTime = diff2;
				}

				if (diff2 > cis->maxMsgTime) {
					cis->maxMsgTime = diff2;
				}

				allTimes += diff2;
				cis->allTimes += diff2;
				msgCounter++;
				cis->msgCounter++;
				msgAvgTime = allTimes / msgCounter;
				cis->msgAvgTime = cis->allTimes / cis->msgCounter;

				if (it->second.missingRecvs == 0) {
					// now message is completed
					if (diff2 < minMsgCTime || minMsgCTime == -1) {
						minMsgCTime = diff2;
					}

					if (diff2 < cis->minMsgCTime || cis->minMsgCTime == -1) {
						cis->minMsgCTime = diff2;
					}

					if (diff2 > maxMsgCTime) {
						maxMsgCTime = diff2;
					}

					if (diff2 > cis->maxMsgCTime) {
						cis->maxMsgCTime = diff2;
					}

					allCTimes += diff2;
					cis->allCTimes += diff2;
					msgCCounter++;
					cis->msgCCounter++;
					msgCAvgTime = allCTimes / msgCCounter;
					cis->msgCAvgTime = cis->allCTimes / cis->msgCCounter;

					unsigned int size = _messages.size();
					_messages.erase(it);
					assert(_messages.size() == size - 1);
				}
			}

			_receivedMsgs.insert(id);
		} else {
			duplicateMsgs++;
			cis->duplicateMsgs++;
		}

		// First message after subscribe

		if ((_state == SUBSCRIBER || _state == PUBSUBER) && (_startSub[cn] >= 0)) {
			oldtime = _startSub[cn];

			diff = newtime - oldtime;

			diff2 = diff.dbl();

			if (diff2 < minFCTime || minFCTime == -1) {
				minFCTime = diff2;
			}

			if (diff2 < cis->minFCTime || cis->minFCTime == -1) {
				cis->minFCTime = diff2;
			}

			if (diff2 > maxFCTime) {
				maxFCTime = diff2;
			}

			if (diff2 > cis->maxFCTime) {
				cis->maxFCTime = diff2;
			}

			allFCTimes += diff2;
			cis->allFCTimes += diff2;
			FCCounter++;
			cis->FCCounter++;
			FCAvgTime = allFCTimes / FCCounter;
			cis->FCAvgTime = cis->allFCTimes / cis->FCCounter;
			_startSub[cn] = -1.0;
		}

		numReceived++;

		EV << "\n" << thisNode.getIp() << ": Got packet from "
	    	           << myMsg->getKey() << ", type:"
	    	           << myMsg->getType()
	    	           << std::endl;

		delete myMsg;
	}

	void TupleFeeder::handleReadyMessage(CompReadyMessage * msg) {
		// if m2etis has been initialized or the OverlayKey is not generated yet, return
		if (m2etis_initialized || overlay->getThisNode().getKey().toString().compare("<unspec>") == 0) {
			delete msg;
			return;
		}

		_id = id++;

		waitForInitMsg = new cMessage("TupleFeeder WaitForInit");
		scheduleAt(simTime() + 0.5, waitForInitMsg);

		if (_state == NONE) {
			if (numRend > 0) {
				_state = RENDEVOUZ;
				numRend--;
			} else if (numSubs > 0) {
				_state = SUBSCRIBER;
				numSubs--;
			} else if (numPubSubs > 0) {
				_state = PUBSUBER;
				numPubSubs--;
			} else if (numPubs > 0) {
				_state = PUBLISHER;
				numPubs--;
			}

			if (numRend + numSubs + numPubSubs + numPubs == 0) {
				startTimeNode = simTime().dbl();
			}
		}

		m2etis_initialized = true;

		delete msg;
	}

	double TupleFeeder::getCurrentLoss() {
		double missing = 0.0;
		unsigned int counter = msgCCounter; // All messages already completed

		for (std::pair<int, AliveMessage> p : _messages) {
			assert(p.second.missingRecvs > 0); // FIXME: mysterious bug caused by dropchance
			if ((simTime() - p.second.sendTime).dbl() >= 1.0) { // only count old messages
				missing += double(p.second.missingRecvs) / p.second.expectedRecvs;
				counter++; // message not yet arrived on all nodes
			}
		}

		if (counter > 0) {
			missing /= counter;
		}

		return missing;
	}

} /* namespace m2etis */
