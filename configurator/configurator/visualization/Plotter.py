__author__ = 'Andreas M. Wahl'

import matplotlib.pyplot as plt
import configurator.util.util as util
from matplotlib import cm
from mpl_toolkits.mplot3d import *
import numpy as np

class Plotter:

    def __init__(self, persistence=None):
        self.persistence = persistence

    def plot2d_from_memory(self, results, x_param, y_param, output_filename, title="", x_label="", y_label=""):
        """
        Plot two-dimensional simulation results from a python data structure in memory.
        :param results: simulation results to be plotted
        :param x_param: parameter of the simulation results to be used on the x axis
        :param y_param: parameter of the simulation results to be used on the y axis
        :param output_filename: file to write the plot to
        :param title: title of the plot
        :param x_label: name of the plot x axis
        :param y_label: name of the y axis
        """
        params = self._get_corresponding_results(util.sanitize_results(results), x_param, y_param)
        self._plot2d(util.strip_unit(params[0]),
                     util.strip_unit(params[1]),
                     output_filename, title, x_label, y_label)

    def plot2d_from_file(self, input_filename, x_param, y_param, output_filename, title="", x_label="", y_label="", c_label=""):
        """
        Plot two-dimensional simulation results from a file in YAML format.
        :param input_filename: file containing the simulation results to be plotted
        :param x_param: parameter of the simulation results to be used on the x axis
        :param y_param: parameter of the simulation results to be used on the y axis
        :param output_filename: file to write the plot to
        :param title: title of the plot
        :param x_label: name of the plot x axis
        :param y_label: name of the plot y axis
        """
        results = self.persistence.retrieve_from_file(input_filename)
        params = self._get_corresponding_results(results, x_param, y_param)
        self._plot2d(util.strip_unit(params[0]),
                     util.strip_unit(params[1]),
                     output_filename, title, x_label, y_label, c_label)

    def plot2d_from_database(self, tag, x_param, y_param, filter, output_filename, title="", x_label="", y_label=""):
        """
        Plot two-dimensional simulation results from a database.
        :param tag: custom tag of the simulation results to be plotted
        :param x_param: parameter of the simulation results to be used on the x axis
        :param y_param: parameter of the simulation results to be used on the y axis
        :param output_filename: file to write the plot to
        :param title: title of the plot
        :param x_label: name of the plot x axis
        :param y_label: name of the plot y axis
        """
        results = self.persistence.retrieve_from_database(self.persistence.config["collection_name"]
            , dict({"tag": tag}.items() + filter.items())
            , {"_id": 0, x_param: 1, y_param: 1}, [(x_param, 1)])

        xs, ys = self._get_corresponding_results(results, x_param, y_param)

        print xs
        print ys

        self._plot2d(util.strip_unit(xs),
                     util.strip_unit(ys),
                     output_filename, title, x_label, y_label)

    def plot3d_from_database(self, tag, x_param, y_param, z_param, filter, output_filename, title="", x_label="", y_label="", z_label=""):

        results = self.persistence.retrieve_from_database(
            self.persistence.config["collection_name"]
            , dict({"tag": tag}.items() + filter.items())
        , {"_id": 0, x_param: 1, y_param: 1, z_param:1}, [(x_param, 1), (y_param, 1)])

        xs, ys, zs = self._get_corresponding_results_3d(results, x_param, y_param, z_param)

        self._plot3d(util.strip_unit(xs),
                     util.strip_unit(ys),
                     util.strip_unit(zs),
                     output_filename, title, x_label, y_label, z_label)

    def _plot3d(self, xs, ys, zs, filename, title="", x_label="", y_label="", z_label="", c_label=""):

        fig = plt.figure()
        ax = fig.gca(projection='3d')               # to work in 3d
        plt.hold = 'True'
        xs, ys = np.meshgrid(xs, ys)
        ax.plot_surface(xs, ys, zs, cmap=cm.hot)
        ax.scatter(xs, ys, zs)
        ax.set_xlabel(x_label)
        ax.set_ylabel(y_label)
        ax.set_zlabel(z_label)

        #plt.close()
        plt.savefig(filename)

    def _plot2d(self, xs, ys, filename, title="", x_label="", y_label="", c_label=""):
        """
        Plot two-dimensional data to a file.
        :param xs: list of x values
        :param ys: list of y values
        :param filename: file to write the plot to
        :param title: title of the plot
        :param x_label: name of the plot x axis
        :param y_label: name of the plot y axis
        """
        plt.plot(xs, ys, label=c_label)
        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.suptitle(title)
        plt.savefig(filename)
        #plt.legend()
        plt.close()
        plt.show()
        
    def _close(self):
        plt.close()

    def _get_corresponding_results(self, results, x_param, y_param):
        resultList = [[] for i in range(0, 2)]

        for result in results:
            for position, parameter in enumerate([x_param, y_param]):
                category = parameter.split('.')[0]
                item = parameter.split('.')[1]
                resultList[position].append(float(util.strip_unit([result[category][item]])[0]))
        #print resultList
        return zip(*sorted(zip(*resultList)))

    def _get_corresponding_results_3d(self, results, x_param, y_param, z_param):

        resultList = [[] for i in range(0, 3)]

        for result in results:
            for position, parameter in enumerate([x_param, y_param, z_param]):
                #print position, parameter
                category = parameter.split('.')[0]
                item = parameter.split('.')[1]
                resultList[position].append(float(util.strip_unit([result[category][item]])[0]))

        return zip(*sorted(zip(*resultList)))
