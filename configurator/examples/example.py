from configurator.Configurator import Configurator
import yaml

config = yaml.load(open("../missile/examples/configs/unified_configuration.mssl", "r"))
conf = Configurator(config)
conf.configure_event("../missile/examples/events/MinimalEvent.mssl", "Latency - All nodes - Avg.mean")

#conf.compute_application_cost_model()
#conf.configure_event_from_application_cost_model("../missile/examples/events/MinimalEvent.mssl", "Latency - All nodes - Avg.mean")

