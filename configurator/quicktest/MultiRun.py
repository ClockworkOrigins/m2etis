__author__ = 'Andreas M. Wahl'


from QuickTest import QuickTest
from RangeMetaGenerator import RangeMetaGenerator
import yaml

path = "quicktest/range_configurations/"

channel_list = ["SIM_DirectBroadcastRouting_NullFilter_DetMergeOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_DetMergeOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_DetMergeOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_DetMergeOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_GMSOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_GMSOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_GMSOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_GMSOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_MTPOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_MTPOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_MTPOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_MTPOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_NullOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_NullOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_NullOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectBroadcastRouting_NullFilter_NullOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_DetMergeOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_DetMergeOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_DetMergeOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_DetMergeOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_GMSOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_GMSOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_GMSOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_GMSOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_MTPOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_MTPOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_MTPOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_MTPOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_NullOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_NullOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_NullOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_DirectRouting_NullFilter_NullOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_DetMergeOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_DetMergeOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_DetMergeOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_DetMergeOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_GMSOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_GMSOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_GMSOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_GMSOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_MTPOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_MTPOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_MTPOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_MTPOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_NullOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_NullOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_NullOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_ScribeRouting_NullFilter_NullOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_DetMergeOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_DetMergeOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_DetMergeOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_DetMergeOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_GMSOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_GMSOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_GMSOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_GMSOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_MTPOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_MTPOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_MTPOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_MTPOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_NullOrder_AckDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_NullOrder_AckDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_NullOrder_NullDeliver_NullPersistence_NullValidity_NullPartition_NullSecurity_NullRendezvous_OMNET",
			"SIM_SpreaditRouting_NullFilter_NullOrder_NullDeliver_NullPersistence_TimeValidity_NullPartition_NullSecurity_NullRendezvous_OMNET"]

numSubs_values = [0]#[1,10,20,40,60,80,100,110,120,130,140,150]
numPubs_values = [0] #[1]
numPubSubs_values = [1,5,10,15,20,25,30,35,40,45,50]
packetSize_values = [16, 256]
numToSend_values = [1,15,30]
fluctuation_values = [0]

gen = RangeMetaGenerator(channel_list, path, numSubs_values, numPubs_values, numPubSubs_values, packetSize_values, numToSend_values, fluctuation_values)
gen.generate_ranges()

for channel in channel_list:
    test = QuickTest(path + channel + ".mtn", "quicktest/config_full.mssl")
    results, fails = test.run_simulations(channel)
    stream = file("results_" + channel + ".yaml", 'w')
    yaml.dump(results, stream)
    stream = file("fails_" + channel + ".yaml", 'w')
    yaml.dump(fails, stream)
