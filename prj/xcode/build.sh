#!/bin/bash

set -e
IOSSDK_VER=$1
xcodebuild -project CrHack.xcodeproj -target CrHack -configuration Release -sdk iphoneos${IOSSDK_VER} build
xcodebuild -project CrHack.xcodeproj -target CrHack -configuration Release -sdk iphonesimulator${IOSSDK_VER} build
cd build
xcrun -sdk iphoneos lipo -create Release-iphoneos/libCrHack.a Release-iphonesimulator/libCrHack.a -output libCrHack.a
xcrun -sdk iphoneos lipo -info libCrHack.a
mv ./libCrHack.a ../../../bin/clang_iphone_all
