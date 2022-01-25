import numpy as np 
import pandas as pd
import altair as alt
from numpy import linalg as LA
from sklearn import datasets
from sklearn.decomposition import PCA

# Téléchargement du jeux de données iris 

data, species = datasets.load_iris(return_X_y=True)

data = pd.DataFrame(data)
species = pd.DataFrame(species)

# Calcul de la matrice de covariance 
cov_matrix = data.cov()

# Calcul des vecteurs et valeurs propres de la matrice de covariance 
eigen_values, eigen_vectors = LA.eig(cov_matrix)

print("eigen_vectors")
print(eigen_vectors)

print("eigen_values")
print(eigen_values)

# Calcul de l'information récupéré en pourcentage sur les 2 premiers axes
info = (eigen_values / sum(eigen_values) * 100).round(2)
axe1_info = info[0]
axe2_info = info[1]

# Projection des points sur les deux premiers vecteurs 
projection_matrix = eigen_vectors.T[:][:2].T
data_t = data.dot(projection_matrix)

# Affichage des nouvelles données à 2 dimensions
data_t.columns = ["axe1", "axe2"]
data_t["species"] = species

chart = alt.Chart(data_t).mark_point().encode(
    x=alt.X("axe1", title=f"axe 1 {axe1_info}%"), 
    y=alt.Y("axe2", title=f"axe 2 {axe2_info}%"), 
    color="species:N")

chart.save('./species.png')    
