#!/usr/bin/env bash

set -e

SDKDIR=./sdk/Adjust
JARINDIR=./sdk/Adjust/adjust/build/outputs
JAROUTDIR=../../Android
PROXYDIR=./proxy
JAR=$JAVA_HOME_7/bin/jar
JAVAC=$JAVA_HOME_7/bin/javac
ANDROIDJAR=$ANDROID_HOME/platforms/android-9/android.jar
VERSION=$(cat ./sdk/VERSION)

(cd $SDKDIR; ./gradlew clean :adjust:makeJar)
#(cd $SDKDIR; mvn clean; mvn package)

cp -v $JARINDIR/adjust-$VERSION.jar $PROXYDIR
mv -v $PROXYDIR/adjust-$VERSION.jar $PROXYDIR/adjust-android.jar; cd $PROXYDIR
$JAVAC -cp "adjust-android.jar:$ANDROIDJAR" com/adjust/sdk/*.java
$JAR uf adjust-android.jar com/adjust/sdk/*.class; cd ..
mv -v $PROXYDIR/adjust-android.jar $JAROUTDIR/adjust-android.jar
rm -v $PROXYDIR/com/adjust/sdk/*.class;
