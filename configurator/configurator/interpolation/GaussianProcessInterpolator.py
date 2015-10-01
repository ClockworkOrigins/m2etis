__author__ = 'amw'

import numpy as np
from sklearn.gaussian_process import GaussianProcess


class GaussianProcessInterpolator:

    def __init__(self, observations):
        self.observations = observations
        self.gaussian_process = GaussianProcess(corr='cubic', theta0=1e-2, thetaL=1e-4, thetaU=1e-1, random_start=100)
        self._compute_model()

    def _compute_model(self):

        observation_points = []
        observation_results = []

        for entry in self.observations:
            observation_points.append(entry[0])
            observation_results.append(entry[1])

        observation_points_array = np.atleast_2d(observation_points)
        observation_results_array = np.array(observation_results).T

        self.gaussian_process.fit(observation_points_array, observation_results_array)

    def compute_prediction(self, observation_points):

        observation_points_array = np.atleast_2d(observation_points)

        predicted_observation_results, MSE = self.gaussian_process.predict(observation_points_array, eval_MSE=True)
        return predicted_observation_results, MSE





