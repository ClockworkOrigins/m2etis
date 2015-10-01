import yaml


class Event(yaml.YAMLObject):
    yaml_tag = u'!Event'

    def __init__(self):
        pass
        #self.Id = Id
        #self.metadata = metadata
        #self.application = application
        #self.payload = payload
        #self.parameters = parameters

    # def validate(self):
    #
    #     self._expand_parameter_presets()
    #     return self._validate_parameter_scalars() and self._validate_payload() and self._validate_dimensions()
    #
    # def _expand_parameter_presets(self):
    #     for preset in self.parameters["presets"]:
    #         for parameter in self.application.presets[preset]:
    #             if parameter in self.parameters["scalars"]:
    #                 logger.info("Ambiguous parameter definition: " + str(parameter) + ". Given values: " + str(
    #                     self.application.presets[preset][parameter]) + "," + str(
    #                     self.parameters["scalars"][parameter]) + " --> Maximum chosen.")
    #                 self.parameters["scalars"].update({
    #                     parameter: max(self.application.presets[preset][parameter],
    #                                    self.parameters["scalars"][parameter])})
    #             else:
    #                 self.parameters["scalars"].update({parameter: self.application.presets[preset][parameter]})
    #
    # def _validate_parameter_scalars(self):
    #     for scalar in self.parameters["scalars"]:
    #         if not (self._validate_parameter_name(scalar) and self._validate_parameter_minimum(
    #                 scalar) and self._validate_parameter_maximum(scalar)):
    #             return False
    #     return True
    #
    # def _validate_payload(self):
    #     payload_keys = self.payload.get("key")
    #     for key in payload_keys:
    #         field = self.payload["fields"][key]
    #         if field is None:
    #             return False
    #     return True
    #
    # def _validate_parameter_name(self, parameter):
    #     application_parameter = self.application.parameters[parameter]
    #     if application_parameter is None:
    #         return False
    #     return True
    #
    # def _validate_parameter_minimum(self, parameter):
    #     application_minimum = self.application.parameters[parameter]["min"]
    #     return self.parameters["scalars"].get(parameter) >= application_minimum
    #
    # def _validate_parameter_maximum(self, parameter):
    #     application_maximum = self.application.parameters[parameter]["max"]
    #     return self.parameters["scalars"].get(parameter) <= application_maximum
    #
    # def _validate_dimensions(self):
    #     return True
    #
    # def generate_payload_cpp(self):
    #     payload_cpp = ""
    #     fields = self.payload["fields"]
    #
    #     for field in fields:
    #
    #         # handle simple field
    #         if isinstance(fields[field], basestring):
    #             print "simple field"
    #
    #         # handle range field
    #         if isinstance(fields[field], list) and "range" in fields[field][1]:
    #                 print "range field"
    #
    #         # handle enum field
    #         if isinstance(fields[field], list) and "values" in fields[field][1]:
    #                 print "enum field"