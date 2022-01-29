# Pca

May be you are familiar with spreadsheets and dynamic cross tables tools to compare columns behaviours as sum,means...but what happens if you have about a thousand columns, you will need a more synthetic view of your datas.  

**Pca**(Principal Component Analysis) is a method attached to Quantitative analysis (QA) branch.  

It performs multidimensional analysis (Rk space), considering "Components" as columns of a datasets.  

Behaviours are calculated as covariance or correlation and represented as 2d square matrix.   
Many of these features yet exists in Python modules, but python may be slow on wide datasets.


The c++ code is a backend to handle large datasets with a best time response.  
Python part [Docker image](./script/python/README.md) can be used to plot and or to crosscheck results directly or from the backend.  

[Matlab/Octave](./script/matlab/README.md) part is available to crosscheck, some 
scripts can be used to generate graphics.

## Purpose

Demistify PCA to let exploration as simple as possible for c/c++ devs.

## Lexical

Pre-processing
* Covariance matrix is the dispersion matrix of a dataset.  
* Correlation matrix is a covariance scaled matrix (identified by diagonal set to 1).  

Svd (Single values decomposition) is the Eigen process applied to a matrix, it returns values and vectors.

Consider 2 forms of Pca
* covariance based  (Svd on unscaled matrix).
* correlation based (Svd on scaled matrix).

As you may notice 
* covariance is lossless with a wide dispersion.
* correlation is lossy with scaled dispersion.

So what should I use cov or cor ?  
When using dataset with columns values of same units use covariance else use correlation.  
So method to use will depend on the nature of your dataset.

## Features

* PCA [fr](https://fr.wikipedia.org/wiki/Analyse_en_composantes_principales) (Principal Component Analysis)
* [Difference between LDA and PCA (en)](https://sebastianraschka.com/faq/docs/lda-vs-pca.html)

## References

### PCA

#### Presentation
 * [Jérome Pages - AgroCampus (fr)](https://www.youtube.com/watch?v=uV5hmpzmWsU)

#### Tools

* [Online Statistics Calculator (en)](https://datatab.net/statistics-calculator/factor-analysis)
* [Principal Component Analysis and Linear Discriminant Analysis with GNU Octave (en)](https://www.bytefish.de/blog/pca_lda_with_gnu_octave.html)

### Pca explaination

* [Math explained PCA](https://www.youtube.com/watch?v=FgakZw6K1QQ)

#### Interpretation
* [Interprétation d'une ACP sur les variables (fr)](http://www.jybaudot.fr/Analdonnees/acpvarres.html)
* [L'analyse en composante principale (fr)](https://dridk.me/analyse-en-composante-principale.html)

#### Questions
 * [Best way to let pca be normalized (en)](https://stats.stackexchange.com/questions/53/pca-on-correlation-or-covariance)

## Fixtures (datasets)
Hereby
* [2x12 inline](src/main.cpp)
* [4x12 pop(gender/salary/age/weight) csv](script/matlab/gsaw.csv)
* [6x23 bovins(vif/carcasse/quality/total/gras/os) csv](script/matlab/bovin.csv)

Sources  
* [Numerical Example](https://www.itl.nist.gov/div898/handbook/pmc/section5/pmc552.htm)
* [2x12 (age/weight)](https://datatab.net/statistics-calculator/factor-analysis)
* [bovins (@see above)](https://cermics.enpc.fr/scilab_new/site/Tp/Statistique/acp/acp.html)

## Requirements

* [CMake](https://cmake.org/). 
* C++ compiler, here g++, [howto change it](https://stackoverflow.com/questions/45933732/how-to-specify-a-compiler-in-cmake) in [CMakeLists.txt](CMakeLists.txt). 
* [Alglib](https://www.alglib.net) included in src. 
* [Boost lib](https://www.boost.org/). 
* [Octave](https://www.gnu.org/software/octave/) or [Matlab](https://mathworks.com/products/matlab.html).

## Build

```
./build.sh
```

## Run

```
./build/pca
```

## Sample output
```
Fixture 2x12
	Fixture datas (matrix)
	          33	          80
	          33	          82.5
	          34	          100.8
	          42	          90
	          29	          67
	          19	          60
	          50	          77
	          55	          77
	          31	          87
	          46	          70
	          36	          57
	          48	          64
	Covariance (matrix)
	          106.727	          10.1182
	          10.1182	          170.026
	Correlation (matrix)
	          1	          0.0751117
	          0.0751117	          1
	Eigen vectors (matrix)
	          -0.154098	          -0.988056
	          -0.988056	          0.154098
	Eigen values (vector)
	          171.604	          105.149
	Explained variance (%)
		P0 62.0061
		P1 37.9939
	Lda
	          0.988056	          -0.154098

```
## Testing

@todo Wip

```
./build/pca_test
```
