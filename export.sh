#!/bin/bash

######################################################################
### Config sript to setup some env variables required for esp dev. ###
######################################################################

if [ "$BASH_SOURCE" = "$0" ]
then
	echo "This scipt should be sourced not run"L
	exit 1
fi

REL_FILE_PATH=$(dirname "${BASH_SOURCE}")

IDF_TARGET=$MCU
ESPPORT=$PORT
DAVANG_ESP=$(cd "${REL_FILE_PATH}"; pwd)
DAVANG_COMPONENT_DIRS=$DAVANG_ESP/components
export IDF_TARGET ESPPORT DAVANG_ESP DAVANG_COMPONENT_DIRS

echo "MCU target: "$IDF_TARGET
echo "Serial port: "$ESPPORT
echo "davang esp env: "$DAVANG_ESP
echo "davang components path:" $DAVANG_COMPONENT_DIRS