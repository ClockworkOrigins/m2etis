import yaml


def yaml_include(loader, node):
    with file(node.value) as document:
        return yaml.load(document)

yaml.add_constructor("!include", yaml_include)
