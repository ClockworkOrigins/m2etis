import yaml

import logging

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)


class Strategy(yaml.YAMLObject):
    yaml_tag = u'!Strategy'

    def __init__(self):
        super

    def compatible(self, event):
        return True

    def preprocess_strategy_combination(self, strategy_combination, event):

        # evaluate blacklist
        if hasattr(self, "blacklist") and not self.blacklist is None:

            for blacklisted_strategy in self.blacklist:

                for strategy in strategy_combination:
                    #print blacklisted_strategy, strategy_combination[strategy].metadata["id"]
                    if strategy_combination[strategy].metadata["id"] == blacklisted_strategy:
                        return False
        else:
            return True
        return True

    def postprocess_strategy_combination(self, strategy_combination, event, results):
        return True

    def __getitem__(self, item):
        return self.__dict__[item]