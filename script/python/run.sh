#!/bin/bash

wpath=$(pwd)/workspace
cwpath=/home/pca/workspace
docker run -v $wpath:$cwpath pypca:0.0.1