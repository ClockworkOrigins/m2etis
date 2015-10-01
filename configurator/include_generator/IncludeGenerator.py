from paramiko import channel

__author__ = 'Andreas M. Wahl'

from string import Template
import logging

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)

ordered_strategy_type_list = ["Routing", "Filter", "Order", "Deliver", "Persistence", "Validity", "Partition",
                              "Security", "Rendezvous"]

NAME_LIST = []
MESSAGE_LIST = []

class IncludeGenerator:

    def __init__(self, config):
        self.config = config

    def generate(self):

        """
        Generate all required c++ header files content
        :return: list containing the code for channel names, event types, channel configurations, message configurations
        """

        # collect the required template substitution information for each generator input item
        #for item in self.config["input"]:
         #   self._set_parameterized_strategies(item)

        # generate and return the c++ header content

        print len(self.config["input"][0]["strategy_combinations"])

        return self._generate_channel_names(self.config["defines"], self.config["input"]), \
            self._generate_event_types(self.config["input"]), \
            self._generate_channel_configurations(self.config["defines"], self.config["input"]), \
            self._generate_message_configurations(self.config["defines"], self.config["input"])

    def prepare_configuration_units(self, configuration_units):
        # collect the required template substitution information for each generator input item
        for configuration_unit in configuration_units:
            self._set_parameterized_strategies(configuration_unit["configuration_unit"])
            self._get_channel_name(configuration_unit["configuration_unit"]),
            self._get_payload_class(configuration_unit["configuration_unit"]),
            self._get_payload_type(configuration_unit["configuration_unit"])

    def _generate_message_configurations(self, defines, input):

        """
        Generate the c++ header content containing the message configurations
        :return: c++ header content
        """

        # initialize template for defines
        define_template_file = open(
            self.config["template_directory"] + "/subtemplates/MessageConfiguration.template", "r"
        )
        define_template_prepared = Template(define_template_file.read())

        # intitialize template for single entries
        entry_template_file = open(self.config["template_directory"] + "/subtemplates/MessageConfigurationEntry.template", "r")
        entry_template_prepared = Template(entry_template_file.read())

        generated_message_configurations = ""

        # process each define one at a time
        elif_defined_required = False
        for define in defines:

            define_name, elif_defined_required = self._get_define_name(define, elif_defined_required)
            message_configurations = []

            # process all generator input items
            for item in input:

                # only generate code for the current define if the event is meant for the current define
                if define["name"] in item["defines"]:
                    strategy_info_exports = []
                    channel_name_exports = []
                    message_configuration_entries = ""

                    # integrate all specified network types
                    for network_type in define["network_types"]:

                        for strategy_combination in item["strategy_combinations"]:

                            parameterized_strategies = self._set_parameterized_strategies(strategy_combination)
                            parameterized_strategies_includes = []

                            # prepend namespace to parameterized strategies
                            for strategy_type in ordered_strategy_type_list:

                                # add the current network type as template parameter to the routing strategy
                                #if strategy_type == "routing":
#                                    parameterized_strategies_includes.append(
#                                        "m2etis::pubsub::routing::"
#                                        + parameterized_strategies[strategy_type]
#                                        + "<" + network_type + ">"
#                                    )
                                #######
                                # TODO: remove temporary fix as soon as partition strategies are properly integrated!
#                                elif strategy_type == "partition":
#                                    parameterized_strategies_includes.append(
#                                        parameterized_strategies[strategy_type]
#                                    )
                                #######

 #                               else:
                                parameterized_strategies_includes.append("m2etis::pubsub::" + strategy_type.lower() + "::" + parameterized_strategies[strategy_type])

                            payload_type = self._get_payload_type(item)

                            # get the last word from a string formatted like "m2etis::net::NETWORKTYPE"
                            channel_name = define["prefix"] + self._get_channel_name(strategy_combination) + "_" + network_type.split("::",4)[4][:-1]

                            # collect channel names to generate boost class exports
                            channel_name_exports.append(channel_name + "Message")

                            ### Temporary workaround to allow simulations:
                            payload_type = "SimulationEventType"

                            # create a new entry by filling out the entry template
                            entry = entry_template_prepared.substitute(
                                network_type=network_type,
                                parameterized_strategies=self._generate_separated_list_string(parameterized_strategies_includes, "\n, "),
                                payload_type=payload_type,
                                channel_name=channel_name
                            )
                            message_configuration_entries += entry + "\n\n"

                        # collect the names of all strategy info files to generate boost class exports
                        for strategy_combination in item["strategy_combinations"]:
                            for strategy_type in ordered_strategy_type_list:
                                if len(strategy_combination[1][strategy_type]["Configuration"]["Configuration Information Class"]) > 0 and not strategy_combination[1][strategy_type]["Metadata Description"]["Identifier"] is None:
                                    strategy_info_exports.append(strategy_combination[1][strategy_type]["Configuration"]["Configuration Information Class"])


                    # fill out the template for the current define
                    defines = define_template_prepared.substitute(
                        define_name=define_name,
                        class_exports_strategy_info=self._generate_info_class_export(set(strategy_info_exports)),
                        class_exports_message=self._generate_message_class_export(sorted(list(set(channel_name_exports)))),
                        message_configuration_entries=message_configuration_entries
                    )
                    message_configurations.append(defines)

            for message_configuration in list(set(message_configurations)):
                generated_message_configurations += message_configuration + "\n"

        # initialize and fill out the main template
        main_template_file = open(self.config["template_directory"] + "/GeneratedMessageConfiguration.h", "r")
        main_template_prepared = Template(main_template_file.read())
        result = main_template_prepared.substitute(
            generated_message_configurations=generated_message_configurations
        )

        return result

    def _generate_channel_configurations(self, defines, input):

        """
        Generate the c++ header content containing the channel configurations
        :return: c++ header content
        """
        generated_channel_configurations = ""

        # initialize the template for single entries
        entry_template_file = open(self.config["template_directory"] + "/subtemplates/ChannelConfiguration.template", "r")
        entry_template_prepared = Template(entry_template_file.read())

        # process each define one at a time

        elif_defined_required = False
        for define in defines:

            channel_configurations = []

            define_name, elif_defined_required = self._get_define_name(define, elif_defined_required)

            # begin a new define
            generated_channel_configurations += "\n" + define_name + "\n"

            # process all generator input items
            for item in input:

                # only generate code for the current define if the event is meant for the current define
                if define["name"] in item["defines"]:

                    # integrate all specified network types
                    for network_type in define["network_types"]:

                        for strategy_combination in item["strategy_combinations"]:

                            parameterized_strategies = self._set_parameterized_strategies(strategy_combination)

                            parameterized_strategies_include = []

                            for strategy_type in ordered_strategy_type_list:

                                # add the current network type as template parameter to the routing strategy
                                #if strategy_type == "Routing":
#                                    parameterized_strategies_include.append(
#                                        parameterized_strategies[strategy_type]
#                                        + "<" + network_type + ">"
#                                    )
 #                               else:
                                parameterized_strategies_include.append(parameterized_strategies[strategy_type])

                            payload_type = self._get_payload_type(item)

                            # get the last word from a string formatted like "m2etis::net::NETWORKTYPE"
                            channel_name = define["prefix"] + self._get_channel_name(strategy_combination) + "_" + network_type.split("::",4)[4][:-1]

                            ### Temporary workaround to allow simulations:
                            payload_type = "SimulationEventType"

                            # create a new entry by filling out the entry template
                            entry = entry_template_prepared.substitute(
                                parameterized_strategies=self._generate_separated_list_string(parameterized_strategies_include, "\n, "),
                                network_type=network_type,
                                payload_type=payload_type,
                                channel_name=channel_name
                            )
                            channel_configurations.append(entry)

            #for channel_configuration in list(set(channel_configurations)):
             #   generated_channel_configurations += channel_configuration + "\n\n"
            generated_channel_configurations += "\n\n".join(list(set(channel_configurations)))

        # initialize and fill out the main template
        main_template_file = open(self.config["template_directory"] + "/GeneratedChannelConfiguration.h", "r")
        main_template_prepared = Template(main_template_file.read())
        result = main_template_prepared.substitute(
            generated_channel_configurations=generated_channel_configurations
        )

        return result


    def _generate_event_types(self, input):

        """
        Generate the c++ header content containing the class definitions for all event payloads
        :return: c++ header content
        """

        # collect the previously generated c++ class code of each generator input item
        payload_classes = []
        for item in input:
                payload_classes.append(self._get_payload_class(item))

        # initialize and fill out the template
        template_file = open(self.config["template_directory"] + "/GeneratedEventTypes.h", "r")
        template_prepared = Template(template_file.read())
        ####
        # Temporary workaround to allow simulations.
        workaround = """
#include "pubsub/filter/events/Position.h"

typedef std::vector<unsigned char> CharVectorEventType;
typedef m2etis::pubsub::filter::Position PositionEventType;

class SimulationEventType {
public:
	SimulationEventType() : _simID(), _simChannel() {
	}

	int _simID;
	int _simChannel;

private:
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & _simID;
			ar & _simChannel;
		}
};
"""
        #result = template_prepared.substitute(payload_classes=self._generate_separated_list_string(list(set(payload_classes)), "\n"))
        result = template_prepared.substitute(payload_classes=workaround)
        return result

    def _generate_channel_names(self, defines, input):

        """
        Generate the c++ header content containing the channel names
        :return: c++ header file content
        """

        # initialize template for single channel entries
        entry_template_file = open(self.config["template_directory"] + "/subtemplates/GeneratedChannelName.template", "r")
        entry_template_prepared = Template(entry_template_file.read())

        entries = ""
        channel_strategy_enum = []

        # process all defines on at a time
        elif_defined_required = False
        for define in defines:

            define_name, elif_defined_required = self._get_define_name(define, elif_defined_required)

            channel_names = []

            #d process all generator input items
            for item in input:
                # only generate code for the current define if the event is meant for the current define
                if define["name"] in item["defines"]:
                    for network_type in define["network_types"]:
                        for entry in item["strategy_combinations"]:
                            # get the last word from a string formatted like "m2etis::net::NETWORKTYPE"
                            generated_channel_name = define["prefix"] + self._get_channel_name(entry) + "_" + network_type.split("::",4)[4][:-1]
                            if not generated_channel_name in channel_names:
                                channel_names.append(generated_channel_name)
                                channel_strategy_enum.append(entry)

            # create a new entry by filling out the entry template
            entries += entry_template_prepared.substitute(
                define_name=define_name,
                channel_names_list=self._generate_separated_list_string( sorted(list(set(channel_names))),", "),
                channel_count=len(list(set(channel_names))),
                channel_names_enum=self._generate_separated_list_string(sorted(list(set(channel_names))), ",\n\t\t\t")
            )

        # initialize the main template
        main_template_file = open(self.config["template_directory"] + "/GeneratedChannelName.h", "r")
        main_template_prepared = Template(main_template_file.read())

        # fill out the main template
        result = main_template_prepared.substitute(
            generated_channel_names=entries
        )

        return result, channel_strategy_enum


    def _get_payload_type(self, item):

        """
        Get the payload type of a generator input item
        :param item: item to be processed
        :return: payload type of the item
        """
        return item["event"].metadata["id"] + "_Payload"


    def _get_payload_class(self, item):

        """
        Get a c++ class describing the payload of a generator input item
        :param item: item to be processed
        :return: c++ class code for the item's payload
        """
        event = item["event"]

        enum_list = []
        field_list = []

        # process all fields
        for field in event.payload["fields"]:
            field_name = field.keys()[0]

            # generate code for each field which is not an enum
            if field[field_name]["type"] != "enum":
                field_list.append(
                    field[field_name]["type"]
                    + " " + field_name
                    + ";"
                )

            # generate code for enum fields
            if field[field_name]["type"] == "enum":
                field_list.append(
                    field_name
                    + "_Enum "
                    + field_name
                    + ";"
                )
                enum_list.append(
                    "enum "
                    + field_name
                    + "_Enum{\n\t\t"
                    + ", ".join(field[field_name]["values"])
                    + "\n\t};"
                )

        # wrap the field and enum definitions in a class definition
        generated_class = \
            "class " \
            + event.metadata["id"] \
            + "_Payload" \
            + "{\n\t" \
            + "\n\t".join(enum_list) \
            + "\n\t" \
            + "\n\t".join(field_list) \
            + "\n};"

        #######
        # TODO: remove temporary fix as soon as content-based routing is properly integrated
        generated_class = "typedef std::vector<unsigned char> " + event.metadata["id"] + "_Payload;"
        #######

        return generated_class


    def _get_channel_name(self, item):

        """
        Get the unique channel name of a generator input item.
        Note: Channel name does not yet contain network type or define-prefix.
        :param item: item to be processed
        :return: channel name of the item
        """
        channel_name = ""
        # create a unique channel name by concatenating the ids of all strategies of the item
        for strategy_type in ordered_strategy_type_list:
            channel_name += item[1][strategy_type]["Metadata Description"]["Identifier"] + "_"
        return channel_name[:-1]

    def _set_parameterized_strategies(self, item):

        """
        Get a mapping of strategy types to the parameterized strategies of a generator input item
        :param item: item to be processed
        :return: of strategy types to parameterized strategies
        """


        entry = {}
        for strategy in item[1]:

            # initialize parameterized strategy
            parameterized_strategy = item[1][strategy]["Metadata Description"]["Identifier"]

            # process strategies without parameters
            if len(item[1][strategy]["Configuration"]["Parameters"]) == 0:
                entry[strategy] = parameterized_strategy

            # process strategies with parameters
            else:

                # open c++ template bracket
                parameterized_strategy += "<"

                # append parameters as c++ template parameters to strategy
                for parameter in item[1][strategy]["Configuration"]["Parameters"]:
                    parameterized_strategy += str(parameter) + ","

                # strip last comma from parameter list
                parameterized_strategy = parameterized_strategy[:-1]

                # close c++ template bracket
                parameterized_strategy += ">"

                entry[strategy] = parameterized_strategy

        return entry


    def _generate_separated_list_string(self, input_list, separator):

        """
        Generates a string in which all items from the input list are separated by the separator string
        :param input_list: list to be converted to a separated string
        :param separator: separator string
        :return: string containing the separated list
        """
        if len(input_list) is 0:
            return ""
        else:
            output = ""
            for i in range(0, len(input_list)):
                if i == len(input_list) - 1:
                    output += input_list[i]
                else:
                    output += input_list[i] + separator

            return output

    def _generate_info_class_export(self, classes):

        """
        Generate boost class exports from a list of class names
        :param classes: list of class names
        :return: string containing the boost class exports
        """
        exports = ""
        for _class in list(set(classes)):
            exports += "BOOST_CLASS_EXPORT(" + _class + ")\n"
            exports += "BOOST_CLASS_IMPLEMENTATION(" + _class + ", boost::serialization::object_serializable)\n"
        return exports

    def _generate_message_class_export(self, classes):

        """
        Generate boost class exports from a list of class names
        :param classes: list of class names
        :return: string containing the boost class exports
        """
        counter = 0
        exports = ""
        for _class in classes:
            exports += "BOOST_CLASS_EXPORT_GUID(" + _class + ',"' + str(counter) + '")\n'
            exports += "BOOST_CLASS_IMPLEMENTATION(" + _class + ", boost::serialization::object_serializable)\n"
            counter += 1
        return exports

    def _get_define_name(self, define, elif_defined_required):
        if define["name"] == "else":
                define_name = "#else"
        else:
            if elif_defined_required is True:
                define_name = "#elif defined " + define["name"]
            else:
                define_name = "#ifdef " + define["name"]
                elif_defined_required = True

        return define_name, elif_defined_required