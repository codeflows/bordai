#! /bin/bash

if [ -z ${BOOST_ROOT} ]; then
  echo "You need to set BOOST_ROOT env var"; exit -1
fi

BASE=$(dirname $0)

# link boost where cinder build expects it to be
ln -s $BOOST_ROOT $BASE/lib/cinder.git/boost

echo "Building Cinder release lib with xcode"
xcodebuild -project $BASE/lib/cinder.git/xcode/cinder.xcodeproj -target cinder -configuration Release

rm $BASE/lib/cinder.git/boost

echo "done."
