__author__ = 'Andreas M. Wahl'

from QuickTest import QuickTest
import yaml

test = QuickTest("quicktest/channel4_all_no_flux.mssl", "quicktest/config.mssl")

results, fails = test.run_simulations("channel4_all_no_flux")

#stream = file('results_channel4_no_flux.yaml', 'w')
#yaml.dump(results, stream)

#stream = file('fails_channel4_no_flux.yaml', 'w')
#yaml.dump(fails, stream)

test.plot2d("channel4_all_no_flux", "parameters.numToSend_4", "results.Control Messages Sendmean", {"parameters.packetSize":"32B", "parameters.numPubSubs": "10"}, "output.png", "title", "x-label", "y-label")




