from configurator.ranges import RangeGenerator

__author__ = 'Andreas M. Wahl'

# initialize generator with config file
generator = RangeGenerator("range_generator_config.m2etis")

# generate simulations in memory
simulations = generator.generate_memory()
print simulations

# generate simulations and save to file
#generator.generate_file("generated_simulations.yaml")

# generate simulations and save to database
# TBD

