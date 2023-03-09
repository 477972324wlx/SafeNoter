#!/bin/bash

RUNNER=$(whoami)

if [ $RUNNER = "root" ]; then
	sudo lsof -i:4396 | grep Server | grep 4396 | awk '{print $2}' | xargs sudo kill -9 &2>/dev/null
else
	echo Should execute in root privellege
	exit
fi
