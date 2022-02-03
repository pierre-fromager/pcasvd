from mlxtend.data import iris_data
from mlxtend.plotting import plot_pca_correlation_graph
import matplotlib.pyplot as plt
import numpy as np

X, y = iris_data()
print("Dataset header")
print(X)

# Normalizing the feature columns is recommended
X_norm = X / X.std(axis=0) 
#X_norm = X 

print("Normalized datas")
print(X_norm)

feature_names = [
  'sepal length',
  'sepal width',
  'petal length',
  'petal width']

figure, correlation_matrix = plot_pca_correlation_graph(X_norm, 
                                                        feature_names,
                                                        dimensions=(1, 2),
                                                        figure_axis_size=10)
figure.savefig('corrcircle.png')