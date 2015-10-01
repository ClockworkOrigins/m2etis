import yaml
import logging

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)

class Network(yaml.YAMLObject):

    yaml_tag = u'!Network'

    def __init__(self):
        super