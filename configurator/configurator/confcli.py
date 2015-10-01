__author__ = 'amw'

import argparse
import sys
from configurator import Configurator


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("configuration", help="Path to the configuration file")
    parser.add_argument("strategies", help="Path to the strategy repository")
    group = parser.add_mutually_exclusive_group()
    group.add_argument("-e", "--event", help="Path to the semantic description for event to be configured")
    group.add_argument("-a", "--application",
                       help="Path to the semantic description for the application to be configured")
    parser.add_argument("-c", "--costmodel", help="Existing cost model to be used for configuration")

    args = parser.parse_args()

    configurator = Configurator(args.strategies)

    if args.event:
        configurator.configureEvent(args.event, args.costmodel)
        sys.exit(0)

    if args.application:
        configurator.calculate_cost_model(args.application)
        sys.exit(0)

    else:
        sys.exit(1)


if __name__ == "__main__":
    main()