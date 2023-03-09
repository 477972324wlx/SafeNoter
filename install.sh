#!/bin/bash

USER=$(whoami)
NOTER_PATH="/home/safe_noter"
NOTER_NAME="safe_noter"

if [ ! $USER = "root" ]; then
   echo Should Execute in root privellege
   exit
fi

echo Compling......

./make.sh

if [ $? -eq 0 ]; then
    echo OK
else
    echo FAIL
fi


echo Creating User ${NOTER_NAME}...

if  useradd safe_noter 2>/dev/null ; then
    echo "User created"
else
    echo "Cannot create User or already existed"
fi


echo creating Folder ${NOTER_PATH}


if mkdir ${NOTER_PATH} 2>/dev/null; then
    echo "System Directory Created!"
else
    echo "Cannot Create Directory Or Already existed"
fi


chown ${NOTER_NAME}:${NOTER_NAME} ${NOTER_PATH} 	

chmod 777 ${NOTER_PATH} 

cp bin/Server ${NOTER_PATH}/Server

echo Starting Server...

wait
cd ${NOTER_PATH}
su - ${NOTER_NAME} -c "./Server &" 


if [ $? -eq 0 ]; then
    echo OK
else
    echo FAIL
fi
