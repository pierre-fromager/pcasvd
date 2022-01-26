#!/bin/bash

docker rmi $(docker images -f "dangling=true" -q) --force
docker rm -v $(docker ps -a -q -f status=exited)
