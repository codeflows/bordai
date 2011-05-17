#! /bin/bash

BASE=$(dirname $0)

$BASE/build-libs.sh

if [ $? -ne 0 ]; then
  exit 1
fi

xcodebuild -project $BASE/xcode/bordai.xcodeproj -target bordai -configuration Release
open $BASE/xcode/build/Release/bordai.app

exit 0
