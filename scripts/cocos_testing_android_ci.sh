#!/usr/bin/env bash

set -e

# Get the current directory (/scripts/ directory)
SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Traverse up to get to the root directory
SDK_DIR="$(dirname "$SCRIPTS_DIR")"
EXAMPLE_DIR=example
RED='\033[0;31m' # Red color
GREEN='\033[0;32m' # Green color
NC='\033[0m' # No Color

cd ${SDK_DIR}
#echo -e "${GREEN}>>> Removing Android JARs${NC}"
#rm -rfv Android/*

#echo -e "${GREEN}>>> Building Android JARs${NC}"
#(cd ext/android; ./build.sh)
#(cd ext/android; ./build_testing.sh)

#echo -e "${GREEN}>>> Building the Android ci testing JAR file ${NC}"
#cd ${SDK_DIR}
#mkdir -p ${EXAMPLE_DIR}/proj.android-studio/app/libs
#cp -rfv Android/adjust-android.jar ${EXAMPLE_DIR}/proj.android-studio/app/libs/
cp -rfv Android/kotlin-test-lib.jar ${EXAMPLE_DIR}/proj.android-studio/app/libs/

echo -e "${GREEN}>>> Running cocos project ${NC}"
cd ${EXAMPLE_DIR}
cocos compile -p android --android-studio
adb install -r proj.android-studio/app/build/outputs/apk/example-debug.apk
