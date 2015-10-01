from symbol import parameters

__author__ = "Andreas M. Wahl"

import re
import logging

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)


class ParameterCollector:

    def __init__(self, config):
        self.config = config

    def _translate_parameters_to_oversim(self, parameters):

        translated_parameters = {}
        for parameter in parameters:
            if parameter in self.config["intuitive_parameter_names"]:
                translated_parameters[self.config["intuitive_parameter_names"][parameter]] = parameters[parameter]
            else:
                translated_parameters[parameter] = parameters[parameter]
        #print translated_parameters
        return translated_parameters

    def collect_parameters_for_application_model(self, application, network, config):

        dynamic_parameters = {}
        for parameter in application.parameters:
            dynamic_parameters[parameter.keys()[0]] = parameter[parameter.keys()[0]]

        static_parameters = {}
        for parameter in network.parameters:
            static_parameters[parameter.keys()[0]] = parameter[parameter.keys()[0]]
        static_parameters.update(config["Simulator"]["technical_parameters"])

        dependent_parameters = {}
        for parameter in config["Simulator"]["dependent_parameters"]:
            dependent_parameters[parameter] = config["Simulator"]["dependent_parameters"][parameter]

        return self._translate_parameters_to_oversim(dynamic_parameters), self._translate_parameters_to_oversim(static_parameters), self._translate_parameters_to_oversim(dependent_parameters)

    def collect_parameters_for_event(self, event, application, network, config):

        required_parameters = []
        for parameter in application.parameters:
            required_parameters.append(parameter.keys()[0])

        available_parameters = {}

        required_parameters, available_parameters = self._evaluate_event_presets(event, application, required_parameters,
                                                                            available_parameters)
        #print available_parameters, required_parameters

        if len(required_parameters) == 0:
            return available_parameters

        required_parameters, available_parameters = self._evaluate_event_scalars(event, application, required_parameters,
                                                                            available_parameters)

        #print available_parameters, required_parameters

        if len(required_parameters) == 0:
            return available_parameters

        required_parameters, available_parameters = self._derive_parameters(event, application, required_parameters,
                                                                       available_parameters)

        #print available_parameters, required_parameters

        if len(required_parameters) == 0:
            return available_parameters

        required_parameters, available_parameters = self._evaluate_application_defaults(event, application, required_parameters,
                                                                                   available_parameters)

        #print available_parameters, required_parameters

        if len(required_parameters) == 0:

            dynamic_parameters = available_parameters

            static_parameters = {}
            for parameter in network.parameters:
                static_parameters[parameter.keys()[0]] = parameter[parameter.keys()[0]]
                static_parameters.update(config["Simulator"]["technical_parameters"])

            dependent_parameters = {}
            for parameter in config["Simulator"]["dependent_parameters"]:
                dependent_parameters[parameter] = config["Simulator"]["dependent_parameters"][parameter]

            return self._translate_parameters_to_oversim(dynamic_parameters), self._translate_parameters_to_oversim(static_parameters), self._translate_parameters_to_oversim(dependent_parameters)

        else:
            return None


    def _evaluate_event_scalars(self, event, application, required_parameters, available_parameters):
        for scalar in event.parameters["scalars"]:
            available_parameters[scalar.keys()[0]] = scalar[scalar.keys()[0]]

        for parameter in available_parameters:
            required_parameters = [p for p in required_parameters if p != parameter]

        return required_parameters, available_parameters


    def _evaluate_event_presets(self, event, application, required_parameters, available_parameters):
        for preset in event.parameters["presets"]:
            app_preset = self._get_list_entry(application.presets, preset)
            preset_parameters = self._get_list_as_dict(app_preset)
            available_parameters = dict(available_parameters.items() + preset_parameters.items())

        for parameter in available_parameters:
            required_parameters = [p for p in required_parameters if p != parameter]

        return required_parameters, available_parameters


    def _derive_parameters(self, event, application, required_parameters, available_parameters):
        available_parameters_updated = True

        while available_parameters_updated:

            available_parameters_updated = False

            for parameter in required_parameters:

                app_parameter = self._get_list_entry(application.parameters, parameter)

                if "formula" in app_parameter[app_parameter.keys()[0]]:

                    result = self._evaluate_formula(app_parameter[app_parameter.keys()[0]]["formula"],
                                               dict(available_parameters))
                    if not result is None:
                        available_parameters[parameter] = result
                        required_parameters = [p for p in required_parameters if p != parameter]
                        available_parameters_updated = True

        return required_parameters, available_parameters


    def _evaluate_application_defaults(self, event, application, required_parameters, available_parameters):
        for parameter in required_parameters:
            app_parameter = self._get_list_entry(application.parameters, parameter)

            if "default" in app_parameter[app_parameter.keys()[0]]:
                available_parameters[parameter] = app_parameter[app_parameter.keys()[0]]["default"]
                required_parameters = [p for p in required_parameters if p != parameter]

        return required_parameters, available_parameters


    def _evaluate_formula(self, formula, parameters):
        variables_regex = '((?:\$([a-z0-9_]*)))'
        variables_regex = re.compile(variables_regex, re.IGNORECASE | re.DOTALL)
        formula_variables = re.findall(variables_regex, formula)

        result_formula = formula
        for variable in formula_variables:

            if variable[1] in parameters:
                result_formula = result_formula.replace(variable[0], str(parameters[variable[1]]))
            else:
                return None

        result_formula = result_formula.replace(" ", "")
        return eval(result_formula, parameters)


    def _get_list_entry(self, list, key):
        for entry in list:
            if entry.keys()[0] == key:
                return entry
        return None


    def _get_list_as_dict(self, list):
        dictionary = {}
        for parameter in list[list.keys()[0]]:
            dictionary[parameter.keys()[0]] = parameter[parameter.keys()[0]]
        return dictionary

