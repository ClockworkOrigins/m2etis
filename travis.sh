#!/usr/bin/env sh
set -evx
env | sort

cd dependencies
./build-boost.sh $M2ETIS_COMPILER
./build-clockUtils.sh $M2ETIS_COMPILER
./build-gflags.sh $M2ETIS_COMPILER
./build-glog.sh $M2ETIS_COMPILER
./build-gmock.sh $M2ETIS_COMPILER
cd ..
mkdir build || true
mkdir build/$M2ETIS_TARGET || true
cd build/$M2ETIS_TARGET
cmake -DWITH_LOGGING=$M2ETIS_LOGGING -DWITH_TESTING=ON -DCMAKE_BUILD_TYPE=$M2ETIS_BUILD_TYPE ../..
make
cd dist/$M2ETIS_BUILD_TYPE

if [ "$M2ETIS_VALGRIND" = "OFF" ]; then 
	./m2etis_unit_tester $M2ETIS_MULTIPLE
	./m2etis_channel_tester
else
	valgrind --leak-check=full ./m2etis_unit_tester $M2ETIS_MULTIPLE
	valgrind --leak-check=full ./m2etis_channel_tester
fi
