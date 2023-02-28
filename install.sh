#!/bin/bash
NOTER_PATH="~/MyNoter"


if  useradd ggbond 2>/dev/null ; then
    echo "User created"
else
    echo "Cannot create User or already existed"
fi


if mkdir $NOTER_PATH 2>/dev/null; then
    echo "Directory Created!"
else
    echo "Cannot Open Directory Or Already existed"
fi
