#! /bin/bash

BASE=$(dirname $0)

if [ ! $BASE/build-libs.sh ]; then
  echo "Library build failed."
  exit 1;
fi

xcodebuild -project $BASE/xcode/bordai.xcodeproj -target bordai -configuration Release
open $BASE/xcode/build/Release/bordai.app
exit 0;
