import copy
import logging
from strategy import  Strategy

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)


class PersistenceStrategy(Strategy):

    def __init__(self, strategy):
        self.__dict__ = copy.deepcopy(strategy.__dict__)

    def compatible(self, event):

        if event is None:
            return True

        if event.dimensions["persistency"]["enabled"] == self.classification["description"]["enabled"]:
            return True
        else:
            return False