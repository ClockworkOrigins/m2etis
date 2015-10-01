__author__ = 'Andreas M. Wahl'

import yaml
import itertools
import logging
from configurator.util.util import strip_unit

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger(__name__)


class RangeGenerator:

    def __init__(self, config):
        #yaml_config = yaml.load(file(config, "r"))
        self.static_fields = config["static_parameters"]
        self.dynamic_fields = config["dynamic_parameters"]
        self.dependent_fields = config["dependent_parameters"]

    def generate_file(self, file_path):
        stream = file(file_path, 'w')
        yaml.dump(self.generate_memory(), stream)

    def generate_memory(self):
        output = list()
        cross_product = self._generate_cross_product_dict()

        if cross_product is None:
            return output

        for dynamic_field in cross_product:
            output_fields = dict()

            # process static fields
            if not self.static_fields is None:
                for field in self.static_fields:
                    output_fields[field] = self.static_fields[field]

            # process dynamic fields
            if not self.dynamic_fields is None:
                field_count = 0
                for field in self.dynamic_fields:
                    #print dynamic_field, field, dynamic_field[field_count]
                    output_fields[field] = dynamic_field[field_count]
                    field_count += 1

            # process dependent fields
            if not self.dependent_fields is None:
                for field in self.dependent_fields:
                    field_description = self.dependent_fields[field]
                    if "unit" in field_description:
                        field_unit = field_description["unit"]
                    else:
                        field_unit = ""

                    if "dependency" in field_description:
			dependency_type = field_description["type"]
			if dependency_type == "sum":
	                        value = 0
        	                for dependency in field_description["dependency"]:
	                            value += strip_unit([output_fields[dependency]])[0]
        	                output_fields[field] = str(value) + str(field_unit)
			if dependency_type == "min":
				minimum = float("infinity")
				for dependency in field_description["dependency"]:
			            value = strip_unit([output_fields[dependency]])[0]
				    minimum = min(minimum, value)
				output_fields[field] = str(minimum) + str(field_unit)    
                    else:
                        LOG.warning("Malformed field found: " + field)
                        return None
            output.append(output_fields)
	
	output = [dict(t) for t in set([tuple(d.items()) for d in output])]

        return output

    def _generate_cross_product_dict(self):

        field_ranges = []

        if self.dynamic_fields is None:
            return None

        for field in self.dynamic_fields:

            field_description = self.dynamic_fields[field]

            # process implicit range field
            if all(k in field_description for k in ("min", "max")):
                field_range = []
                range_from = field_description["min"]
                range_to = field_description["max"]
                range_element = range_from
                field_unit = field_description["unit"]

                field_range.append(range_element)
                while range_element < range_to:
                    if "step" in field_description:
                        eval_result = eval(field_description["step"]["function"], dict({"PREVIOUS": range_element}, **field_description["step"]["parameters"]))
                        if eval_result <= range_to:
                            range_element = eval_result
                            field_range.append(str(range_element) + str(field_unit))
                        else:
                            break
                    else:
                        eval_result = range_element + 1
                        if eval_result <= range_to:
                            range_element = eval_result
                            field_range.append(str(range_element) + str(field_unit))
                        else:
                            break
                field_ranges.append(field_range)

            # process explicit range field
            elif "values" in field_description:
                field_unit = field_description["unit"]
                values = field_description["values"]
                value_list = []
                for value in values:
                    if not field_unit is None:
                        value_list.append(str(value) + str(field_unit))
                    else:
                        value_list.append(value)

                field_ranges.append(value_list)

            # abort on malformed fields
            else:
                LOG.warning("Malformed field found: " + field)
                return None

        output = itertools.product(*field_ranges)
        return list(output)

