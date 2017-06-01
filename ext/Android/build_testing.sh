#!/usr/bin/env bash

set -e

SDKDIR=./sdk/Adjust
JARINDIR=./sdk/Adjust/testlibrary/build/outputs
JAROUTDIR=../../Android
PROXYDIR=./proxy_testing
JAR=$JAVA_HOME_7/bin/jar
JAVAC=$JAVA_HOME_7/bin/javac
ANDROIDJAR=$ANDROID_HOME/platforms/android-9/android.jar

(cd $SDKDIR; ./gradlew clean :testlibrary:makeJar)

cp -v $JARINDIR/adjust-testing.jar $PROXYDIR
cd $PROXYDIR
$JAVAC -cp "adjust-testing.jar:$ANDROIDJAR" com/adjust/testlibrary/*.java
$JAR uf adjust-testing.jar com/adjust/testlibrary/*.class; cd ..
mv -v $PROXYDIR/adjust-testing.jar $JAROUTDIR/adjust-testing.jar
rm -v $PROXYDIR/com/adjust/testlibrary/*.class;
