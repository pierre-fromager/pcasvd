#  Python docker

Docker image builder embedding most used python modules to plot and compute

* Pca
* Lda
* Correlation
* ...

## Modules

* numpy
* pandas
* altair
* altair_saver
* sklearn
* mlxtend
* matplotlib

Feel free to add your owns in **requirements.txt**.  
If changes => build.

## Npm packages

Some python modules required node libs to save figures, see **package.jon**.  
If changes => build.

## Sizes

* Python modules (460Mo)
* Node modules (250Mo)
* python:3.9-slim-bullseye image (250Mo)
* Debian extra packages (20Mo)

## Requirements

* Docker

## Build

Simply 
```
./build.sh
```

Clean exited containers
```
./clean.sh
```

## Run

Default user is pca change -u option to use root.

Interractive

```
./runit.sh
```

One shot

```
./run.sh
```

## Volumes

* Workspace is mounted on $HOME/pca