import copy
import logging
from strategy import  Strategy

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)


class PartitionStrategy(Strategy):

    def __init__(self, strategy):
        self.__dict__ = copy.deepcopy(strategy.__dict__)