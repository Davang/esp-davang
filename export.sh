#!/bin/bash

#############################################################################
### Config sript to setup some env variables required for esp-davang dev. ###
#############################################################################

if [ "$BASH_SOURCE" = "$0" ]
then
	echo "This scipt should be sourced not run"L
	exit 1
fi

REL_FILE_PATH=$(dirname "${BASH_SOURCE}")

DAVANG_ESP=$(cd "${REL_FILE_PATH}"; pwd)
DAVANG_COMPONENT_DIRS=$DAVANG_ESP/components
export DAVANG_ESP DAVANG_COMPONENT_DIRS

echo "davang esp env: "$DAVANG_ESP
echo "davang components path:" $DAVANG_COMPONENT_DIRS