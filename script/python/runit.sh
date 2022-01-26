#!/bin/bash

wpath=$(pwd)/workspace
cwpath=/home/pca/workspace
docker run -u pca -i -t -v $wpath:$cwpath pypca:0.0.1 /bin/bash