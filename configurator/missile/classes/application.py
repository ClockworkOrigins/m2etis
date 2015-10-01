import yaml
import logging

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)


class Application(yaml.YAMLObject):

    yaml_tag = u'!Application'

    def __init__(self):
            super