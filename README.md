# PcaSdv

The idea is to calculate :
* PCA(Principal Component Analysis) 
* SVD(Singular Value Decomposition) 
For the sake I want to check results with Boost unit tests.  

Here I use [Alglib for statistics computation](https://www.alglib.net/statistics) and I do cross check with Octave(for the math evaluation) then Boost lib.

## References

### PCA

 * [Numerical Example](https://www.itl.nist.gov/div898/handbook/pmc/section5/pmc552.htm)
 * [Online Statistics Calculator](https://datatab.net/statistics-calculator/factor-analysis)
 * [Interprétation d'une ACP sur les variables](http://www.jybaudot.fr/Analdonnees/acpvarres.html)
 * [Principal Component Analysis and Linear Discriminant Analysis with GNU Octave](https://www.bytefish.de/blog/pca_lda_with_gnu_octave.html)

### SDV

## Requirements

* CMake. 
* C++ compiler (g++). 
* Alglib (included in). 
* Boost lib. 

## Build

```
./build.sh
```

## Run

```
./build/pca
```

## Testing

@todo Wip

```
./build/pca_test
```
