# Installation script

RELEASE_FOLDER='release'
mkdir -p $RELEASE_FOLDER
cd $RELEASE_FOLDER || exit
cmake ../ -DCMAKE_BUILD_TYPE=Release

cd .. || exit

echo '-----------'

DEBUG_FOLDER='debug'
mkdir -p $DEBUG_FOLDER
cd $DEBUG_FOLDER || exit
cmake ../ -DCMAKE_BUILD_TYPE=Debug

cd .. || exit