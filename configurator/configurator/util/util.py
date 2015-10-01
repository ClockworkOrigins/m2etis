# utilities

import string


def sanitize_dict( _dict):
    clean = {}
    for item in _dict:
        new_item = item.translate(None, "*")
        new_item = new_item.translate(None, ".")
        clean[new_item] = _dict[item]
    return clean


def sanitize_results(results):
    for result in results:
        result["parameters"] = sanitize_dict(result["parameters"])
        result["results"] = sanitize_dict(result["results"])
    return results


def strip_unit(values):
    result = []
    for value in values:
        value = str(value)
        value = float(value.translate(None, string.ascii_letters))
        result.append(value)
    return result
