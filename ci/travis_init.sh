#!/bin/bash
set -e

currDir=$(dirname $0)

if [[ $TRAVIS_OS_NAME == "linux" ]]; then	
	# append post build script
	echo "$currDir/travis_postbuild.sh" >> qtmodules-travis/ci/linux/build-docker.sh
fi
