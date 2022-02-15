
# Eigens visualisation init from :
# https://alyssaq.github.io/2015/understanding-eigenvectors-and-eigenvalues-visually/
import numpy as np
import matplotlib.pyplot as plt
# Plot points for a given matrix
def plot_points(matrix, ls='--', lw=1.2, colors=None):
  x_points, y_points = matrix
  size = len(x_points)
  colors = ['plum', 'm', 'firebrick', 'turquoise'] if not None else colors
  for i in range(size):
    plt.plot(x_points[i], y_points[i], color=colors[i], marker='o')
    plt.plot([x_points[i], x_points[(i+1) % size]], 
             [y_points[i], y_points[(i+1) % size]], 
             color=colors[i], linestyle=ls, linewidth=lw)
# Draw label for a given point with arrow    
def draw_point_label(prefix, point, location):
  plt.annotate('{0}={1}'.format(prefix, tuple(map(lambda x: round(x, 2), point))), 
  xy=point, xytext=location, textcoords='data', color='white', weight='normal',
  bbox=dict(fc='black', alpha=0.6, ec='none'), arrowprops=dict(arrowstyle='->'))
# intial rectangle points (4pts) 
# represented by 2x1d array xy(pts) by columns (-10,-10),(-10,20)...
xpts = np.array([-10, -10, 20, 20])
ypts = np.array([-10, 20, 20, -10])
matrix = np.array([xpts, ypts])    
# Transfo matrix used for projection
A = np.matrix([[1, 0.3], [0.45, 1.2]])
# Projection => transfo (scalar product)
transformed_matrix = A * matrix
# Set orthogonal axis
plt.axis('off') # remove default
ax = plt.axes() # define new axis
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')
ax.yaxis.set_ticks_position('left')
ax.set_aspect('equal')
# Plot initial rectangle points from matrix
plot_points(matrix)
# Plot projected rectangle points from transformation matrix
plot_points(transformed_matrix.A, '-', lw=3.0)
# Svd
evals, evecs = np.linalg.eig(A)
# Calculate 2 line's points for each eigen vectors
x_v1, y_v1 = evecs[:,0].getA1()
x_v2, y_v2 = evecs[:,1].getA1()
# Calculate gradient(slope)
m1 = y_v1/x_v1  # from 1st eigenvector
m2 = y_v2/x_v2  # from 2nd eigenvector
# Determine points from vectors
px1 = -10 # arbitrary value could be whatever
p1 = [px1/m1, px1] # 1st point for y = px1
print("p1:",p1)
px2 = 20 # arbitrary value could be whatever
p2 = [px2/m2, px2] # 2nd point for y = px2
print("p2:",p2)
# Project points : multiply point vector by matrix transfo A
trans_p1 = A*np.matrix(p1).T
print("T(p1):[",trans_p1[0,0],",",trans_p1[1,0],"]")
trans_p2 = A*np.matrix(p2).T
print("T(p2):[",trans_p2[0,0],",",trans_p2[1,0],"]")
# Plot eigenvectors(lines) with labels
escale = 50
plt.plot([x_v1*-escale, x_v1*escale], [y_v1*-escale, y_v1*escale], color='royalblue')
plt.plot([x_v2*-escale, x_v2*escale], [y_v2*-escale, y_v2*escale], color='crimson')
plt.annotate('e1', xy=(-20, 14),textcoords='data', weight='normal', color='royalblue')
plt.annotate('e2', xy=(-11, -20),textcoords='data', weight='normal', color='crimson')
# Plot the points where the eigenvector line and original points intersect
plt.plot(p1[0], p1[1], 'ko')
plt.plot(p2[0], p2[1], 'ko')
# Plot the transformed points that lie on the eigenvector line
plt.plot(trans_p1[0,0], trans_p1[1,0], 'ko')
plt.plot(trans_p2[0,0], trans_p2[1,0], 'ko')
# Plot the point labels
draw_point_label('p1', p1, (4, -19))
draw_point_label('T(p1)', trans_p1.A1, (18, -7))
draw_point_label('p2', p2, (-14, 26))
draw_point_label('T(p2)', trans_p2.A1, (-6, 33))
# Limit the plot
plt.xlim([-20, 30])
plt.ylim([-20, 35])
# Save figure
plt.savefig('draweigens.png')