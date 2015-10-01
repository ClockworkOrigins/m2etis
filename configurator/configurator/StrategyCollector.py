__author__ = 'amw'

import yaml
import itertools
import uuid

class StrategyCollector:

    def __init__(self, dimensions_config, event, strategies):
        self.dimensions_config = dimensions_config
        self.event = event
        self.strategy_repository = strategies
        self.initial_strategy_combinations = None

    def get_initial_strategy_combinations(self):

        # collect all available and suitable strategies
        strategies = {}
        for dimension in self.dimensions_config["formal_dimensions"]:

            #if dimension in self.event.dimensions:
            strategies.update(self._get_strategies_by_dimension(dimension))
            #else:
             #   pass
                # TODO: event not completely specified

        # generate all possible strategy combinations
        strategy_ranges = []
        for dimension in strategies:
            strategy_range = strategies[dimension]
            strategy_ranges.append(strategy_range)


        strategy_crossproduct = list(itertools.product(*strategy_ranges))

        strategy_combinations = []
        for entry in strategy_crossproduct:
            entry_list = list(entry)
            strategy_combination = {}
            for strategy in entry_list:
                strategy_combination[strategy.Classification["Type"]] = strategy
            strategy_combinations.append(strategy_combination)

        self.initial_strategy_combinations = strategy_combinations
        return strategy_combinations

    def _get_strategies_by_dimension(self, dimension):

        strategies = {}

        strategy_types = self.dimensions_config["formal_strategy_types"][dimension]
        for strategy_type in strategy_types:

            m2etis_strategy_types = self.dimensions_config["m2etis_strategy_types"][strategy_type]
            for m2etis_strategy_type in m2etis_strategy_types:
                strategies[m2etis_strategy_type] = self._get_strategies_by_type(m2etis_strategy_type)

        return strategies

    def _get_strategies_by_type(self, type):
        strategies = []
        for strategy in self.strategy_repository:
            if (strategy.Classification["Type"] == type): #and (strategy.compatible(self.event)):
                strategies.append(strategy)

        return strategies

    def preprocess_strategy_combinations(self):

        if self.initial_strategy_combinations is None:
            self.get_initial_strategy_combinations()

        approved_strategy_combinations = []

        for strategy_combination in self.initial_strategy_combinations:
            approved = True
            for strategy_type in strategy_combination:

                if not strategy_combination[strategy_type].preprocess_strategy_combination(strategy_combination, self.event):
                    approved = False

            if approved:
                approved_strategy_combinations.append([str(uuid.uuid1()), strategy_combination])

        return approved_strategy_combinations

    def postprocess_strategy_combinations(self, results):

        approved_strategy_combinations = []

        for strategy_combination in self.initial_strategy_combinations:

            approved = True
            for strategy_type in strategy_combination:
                if not strategy_combination[strategy_type].postprocess_strategy_combination(strategy_combination, self.event, results):
                    approved = False

            if approved:
                approved_strategy_combinations.append(strategy_combination)

        return approved_strategy_combinations