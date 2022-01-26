# Pca

The idea is to propose most used primitives for Quantitative analysis (QA).  
Many of these features yet exists in Python modules, but python may be slow for huge calculus.  
The c++ code is a backend to handle large scale calculus.  
The python part through the [Docker image](./script/python/README.md) can be used to plot and or to crosscheck results directly or from the backend.  
Thus a [Matlab/Octave](./script/matlab/README.md) part is available to crosscheck, some script can be used to generate graphics.

## Features

* PCA [fr](https://fr.wikipedia.org/wiki/Analyse_en_composantes_principales) (Principal Component Analysis)
* [Difference between LDA and PCA](https://sebastianraschka.com/faq/docs/lda-vs-pca.html)

## References

### PCA

#### Tools

* [Online Statistics Calculator](https://datatab.net/statistics-calculator/factor-analysis)
* [Principal Component Analysis and Linear Discriminant Analysis with GNU Octave](https://www.bytefish.de/blog/pca_lda_with_gnu_octave.html)

#### Interpret
 * [Interprétation d'une ACP sur les variables](http://www.jybaudot.fr/Analdonnees/acpvarres.html)
 
#### Presentation
 * [Jérome Pages - AgroCampus](https://www.youtube.com/watch?v=uV5hmpzmWsU)

#### Questions
 * [Best way to let pca be normalized](https://stats.stackexchange.com/questions/53/pca-on-correlation-or-covariance)

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
* [Alglib](https://www.alglib.net). 
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
