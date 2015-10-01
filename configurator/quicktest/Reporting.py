__author__ = 'amw'

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from configurator.persistence.PersistenceManager import PersistenceManager
import configurator.util.util as util
from scipy.interpolate import griddata
from configurator.util.util import sanitize_results
import yaml

def create_plot_matrix(datasets, type, numRows, numCols, title, labels):
    plt.close('all')
    fig = plt.figure()
    fig.suptitle(title)
    for i in range(numRows*numCols):

        if i >= len(datasets):
            break

        dataset = datasets[i]

        if type == "2d":
            ax = plt.subplot(numRows,numCols,i)
            ax.plot(dataset[0], dataset[1])
            ax.set_title(dataset[2])
            ax.set_xlabel(labels[0])
            ax.set_ylabel(labels[1])

        if type == "3d":
            #plt.gca().invert_xaxis()

            ax = plt.subplot(numRows,numCols, i, projection='3d')

            ax.set_xlabel(labels[0])
            ax.set_ylabel(labels[1])
            ax.set_zlabel(labels[2])

            x = dataset[0]
            y = dataset[1]
            z = dataset[2]

            xi = np.linspace(min(x), max(x),100)
            yi = np.linspace(min(y), max(y),100)
            zi = griddata((x, y), z, (xi[None,:], yi[:,None]))
            xig, yig = np.meshgrid(xi, yi)

            ax.plot_surface(xig,yig,zi)
            ax.scatter(x,y,z, color="red")
            ax.set_title(dataset[3])

    plt.show()


def _get_corresponding_results(results, x_param, y_param):

        resultList = [[] for i in range(0, 2)]

        for result in results:
            for position, parameter in enumerate([x_param, y_param]):
                category = parameter.split('.')[0]
                item = parameter.split('.')[1]
                resultList[position].append(float(util.strip_unit([result[category][item]])[0]))

        return zip(*sorted(zip(*resultList)))

def _get_corresponding_results_3d(results, x_param, y_param, z_param):

        resultList = [[] for i in range(0, 3)]

        for result in results:
            for position, parameter in enumerate([x_param, y_param, z_param]):
                #print position, parameter
                category = parameter.split('.')[0]
                item = parameter.split('.')[1]
                resultList[position].append(float(util.strip_unit([result[category][item]])[0]))

        return zip(*sorted(zip(*resultList)))

def create_dataset(persistence, type, tag, params, filter):

    if type == "2d":
        results = persistence.retrieve_from_database(persistence.config["collection_name"]
                , dict({"tag": tag}.items() + filter.items())
                , {"_id": 0, params[0]: 1, params[1]: 1}, [(params[0], 1)])

        xs, ys = _get_corresponding_results(results, params[0], params[1])

        title = ""
        for item in filter:
            title = title + " " + str(item.split(".")[1]) + ":" + str(filter[item])

        dataset = [xs,ys, title]

    if type == "3d":
        results = persistence.retrieve_from_database(persistence.config["collection_name"]
                , dict({"tag": tag}.items() + filter.items())
                , {"_id": 0, params[0]: 1, params[1]: 1, params[2]: 1}, [(params[0], 1)])

        xs, ys, zs = _get_corresponding_results_3d(results, params[0], params[1], params[2])

        title = ""
        for item in filter:
            title = title + " " + str(item.split(".")[1]) + ":" + str(filter[item])

        dataset = [xs, ys, zs, title]

    return dataset


def plot2d(db_config, tag, x_param, y_param, filter_list, numRows, numCols, title):
    persistence = PersistenceManager(db_config)
    persistence.initialise_database_client()

    datasets = []

    for filter in filter_list:
        dataset = create_dataset(persistence, "2d", tag, [x_param, y_param], filter)
        datasets.append(dataset)

    create_plot_matrix(datasets, "2d", numRows, numCols, title, [str(x_param.split(".")[1]), y_param.split(".")[1]])

def plot3d(db_config, tag, x_param, y_param, z_param, filter_list, numRows, numCols, title):
    persistence = PersistenceManager(db_config)
    persistence.initialise_database_client()

    datasets = []

    for filter in filter_list:
        dataset = create_dataset(persistence, "3d", tag, [x_param, y_param, z_param], filter)
        datasets.append(dataset)

    create_plot_matrix(datasets, "3d", numRows, numCols, title, [str(x_param.split(".")[1]), y_param.split(".")[1], z_param.split(".")[1]])


