
# https://www.alldatascience.com/classification/wine-dataset-analysis-with-python/

import numpy as np
import pandas as pd
import seaborn as sns
from sklearn import datasets
from sklearn.manifold import TSNE
from sklearn.decomposition import PCA
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, confusion_matrix
from sklearn.model_selection import train_test_split, cross_val_score

import matplotlib.pyplot as plt

pd.set_option('display.max_columns', None)

# %matplotlib inline

#Let's import the data from sklearn
from sklearn.datasets import load_wine
wine=load_wine()

#Convert to pandas dataframe
data=pd.DataFrame(data=np.c_[wine['data'],wine['target']],columns=wine['feature_names']+['target'])

#Check data with info function
data.info()

data.to_csv('wine_init.csv')