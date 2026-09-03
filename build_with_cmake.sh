#!/bin/bash

# handle argv
if [ "$#" -lt 1 ]; then
	echo "[WARNING] build_with_cmake.sh without build type"
	echo "[INFO] Usage: build_with_cmake.sh <Debug|Release|RelWithDebInfo>"
	echo "[INFO] By default, set BUILD_TYPE=release"

	BUILD_TYPE=release
else
	# to lowercase
	BUILD_TYPE=$(echo $1 | tr '[:upper:]' '[:lower:]')
fi

origin_dir="$(dirname "$(readlink -f "$0")")"
build_dir=$origin_dir/build
dist_dir=$origin_dir/dist

cd $origin_dir

if [ -d $build_dir ]; then
	rm -rf $build_dir
fi
if [ -d $dist_dir ]; then
	rm -rf $dist_dir
fi

mkdir -p $build_dir
cmake \
	-S $origin_dir -B $build_dir \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	-DBUILD_SHARED_LIBS=ON \
	-DHACLOG_BUILD_EXAMPLE=OFF \
	-DBUILD_TESTING=OFF \
	-DHACLOG_BUILD_BENCHMARK=OFF \
	-DHACLOG_BUILD_GBENCHMARK=OFF \
	-DCMAKE_INSTALL_PREFIX=$dist_dir

# make && make install
cmake --build $build_dir
cmake --build $build_dir --target install
