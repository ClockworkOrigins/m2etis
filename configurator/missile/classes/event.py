import yaml
import logging

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)


class Event(yaml.YAMLObject):
    yaml_tag = u'!Event'

    def __init__(self):
        super

    def validate(self):
        return True