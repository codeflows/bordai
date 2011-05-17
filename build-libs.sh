#! /bin/bash

if [ -z ${BOOST_ROOT} ]; then
  echo "You need to set BOOST_ROOT env var"; exit -1
fi

BASE=$(dirname $0)

# link boost where cinder build expects it to be
ln -s $BOOST_ROOT $BASE/lib/cinder.git/boost

echo "Building Cinder release lib with xcode"
xcodebuild -project $BASE/lib/cinder.git/xcode/cinder.xcodeproj -target cinder -configuration Release
xcodebuild -project $BASE/lib/cinder.git/xcode/cinder.xcodeproj -target cinder -configuration Debug

rm $BASE/lib/cinder.git/boost

echo "Setting link to boost under libs for"
if [ -d ${BASE}/lib/boost ]; then
  rm $BASE/lib/boost
fi
ln -s $BOOST_ROOT $BASE/lib/boost

echo "done."
