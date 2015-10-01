__author__ = 'Andreas M. Wahl'

import logging
from pymongo import MongoClient
import yaml
import configurator.util.util as util
import subprocess

logging.basicConfig(level=logging.WARN)
LOG = logging.getLogger(__name__)


class PersistenceManager:

    def __init__(self, config):
        """
        :param config: dictionary containing database configuration information
        """
        self.config = config
        self.db = None

    # file persistence:

    def persist_to_file(self, results, file_path):
        """
        Persist simulation results to a file in YAML format.
        :param results: simulation results to be persisted
        :param file_path: file to which the simulation results are written
        """
        stream = file(file_path, 'w')
        yaml.dump(util.sanitize_results(results), stream)

    def retrieve_from_file(self, file_path):
        """
        Retrieve simulation results from a file in YAML format.
        :param file_path: file from which the simulation results are retrieved
        :return: retrieved simulation results
        """
        return yaml.load(file(file_path, "r"))

    # database persistence:

    def start_db(self):

        command = "/home/amw/uni/m2etis/m2etis/dependencies/persistence/mongodb/bin/mongod --dbpath /home/amw/uni/m2etis/m2etis/dependencies/persistence/data"
        args = command.split(" ")

        mongod = subprocess.Popen(args,stdout=subprocess.PIPE)

        for line in iter(mongod.stdout.readline, ''):
            if "[initandlisten] waiting for connections on port" in line:
                break

        try:
            client = MongoClient()
            self.db = client[self.config["db_name"]]
        except Exception as ex:
            LOG.warning(str(ex))
            self.db = None

    def shutdown_db(self):
        subprocess.Popen(["killall", "mongod"])

    def initialise_database_client(self):
        try:
            client = MongoClient()
            self.db = client[self.config["db_name"]]
        except Exception as ex:
            LOG.warning(str(ex))
            self.db = None

    def persist_to_database(self, collection, results):
        """
        Persist simulation results to the database configured in the class constructor.
        :param results: retrieved simulation results
        """
        if not self.db is None:
            self.db[collection].insert(results)#util.sanitize_results(results))
        else:
            LOG.warning("No database connection specified. Aborting current database operation.")

    def retrieve_from_database(self, collection, query, projection, sort):
        """
        Retrieve simulation results from the database configured in the class constructor.
        :param query: dictionary containing keys to be used for querying the database for suitable documents
        :param projection: dictionary with simulation result fields to be included in the query output. field value of "1" means include, "0" means omit the field
        :param sort: list of dictionaries containing fields to determine the order of the database output. each dictionary contains only one field. value of "1" means sort ascending, "-1" means sort descending
        :return: list of retrieved simulation results
        """
        if not self.db is None:
            return self.db[collection].find(query, projection).sort(sort)
        else:
            LOG.warning("No database connection specified. Aborting current database operation.")
            return []

    def clear_database(self, collection):
        """
        Delete all entries from the database configured in the class constructor.
        """
        if not self.db is None:
            self.db[collection].remove()
        else:
            LOG.warning("No database connection specified. Aborting current database operation.")

    # TODO: currently intensionally not working
    def retrieve_from_database_corresponding_results(self, tag, x_param, y_param):
        """
        Retrieve two-dimensional corresponding and sorted simulation results from the database configured in the class constructor.
        :param tag: tag of the simulation results to be retrieved
        :param x_param: name of the x parameter
        :param y_param: name of the y parameter
        :return: list of two lists containing the x and y values of the retrieved simulation results
        """
        if not self.db is None:
            cursor = self.retrieve_from_database(self.config["collection_name"], {"tag": tag}, {"_id": 0, x_param: 1, y_param: 1}, [(x_param, 1)])
            x_list = []
            y_list = []
            for doc in cursor:
                x_category = x_param.split('.')[0]
                x_item = x_param.split('.')[1]
                x_list.append(doc[x_category][x_item])

                y_category = y_param.split('.')[0]
                y_item = y_param.split('.')[1]
                y_list.append(doc[y_category][y_item])
            return x_list, y_list
        else:
            LOG.warning("No database connection specified. Aborting current database operation.")
            return [], []