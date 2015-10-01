import yaml


def yaml_include(loader, node):
    with file(node.value) as document:
        return yaml.load(document)

def yaml_ref(loader, node):
    print node

def yaml_key(loader, node):
    print node

yaml.add_constructor("!include", yaml_include)
yaml.add_constructor("!ref", yaml_ref)
yaml.add_constructor("!key", yaml_key)
