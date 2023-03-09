#!/bin/bash

NOTER_PATH="/home/safe_noter"
NOTER_NAME="safe_noter"
RUNNER=$(whoami)

if [ ! $RUNNER = "root" ]; then
	echo Should execute in root privellege
	exit
fi

cd ${NOTER_PATH}
su - ${NOTER_NAME} -c "./Server &" 


if [ $? -eq 0 ]; then
    echo OK
else
    echo FAIL
fi

