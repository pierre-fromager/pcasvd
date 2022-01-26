#!/bin/bash

wpath=$(pwd)/workspace
cwpath=/home/pca/workspace
docker run -i -t -v $wpath:$cwpath pypca:0.0.1 /bin/bash