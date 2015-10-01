__author__ = 'amw'


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

event2 = yaml.load(file("event.missile","r"))

print event2
#print event2.Payload

#fields = event2.Payload["Fields"]
#print fields
#for field in fields:
    #print field
 #   key = field.keys()[0]
#    print field[key]
