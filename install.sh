#!/bin/bash
NOTER_PATH="~/MyNoter"
a=3

NOTER_PATH="/home/safe_noter"
NOTER_NAME="safe_noter"

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


chown ${NOTER_NAME} ${NOTER_PATH} 	
chmod 600 ${NOTER_PATH} 

cp bin/Server ${NOTER_PATH}/Server

cd ${NOTER_PATH} && ./Server
