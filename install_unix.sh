#!/bin/bash
# First check whether the installation of DIAMONDS is required. Give it priority with respect to
# the other software packages.
flag1=0
flag2=0

if [ $# -eq 0 ]; then
    echo "Usage: ./install_unix.sh [--diamonds | -d] [--background | -b]"
    exit 1
fi

if ! [ -x "$(command -v git)" ]; then
	echo "Error: git is not installed. Aborting..." >&2
	exit 1
fi

if ! [ -x "$(command -v cmake)" ]; then
	echo "Cmake is not installed. Trying to install it using apt-get..." >&2

	if ! [ -x "$(command -v apt-get)" ]; then
		echo "Error: apt-get is not installed. Aborting..." >&2
		exit 1
	else
		sudo apt-get install cmake
	fi
fi

while [ ! $# -eq 0 ]
do
	case "$1" in
		--diamonds | -d)
			flag1=1
			;;
		--background | -b)
			flag2=1
			;;
		*) 
			echo "Flag $1 not recognized. Only flags -d -b are allowed. Aborting..."
			exit 1
			;;
	esac
	shift
done

if [ $flag1 -eq 1 ]; then
	echo "-----------------------------------"
	echo " Cloning and installing DIAMONDS..."
	echo "-----------------------------------"
	git clone https://github.com/EnricoCorsaro/DIAMONDS.git
	mkdir DIAMONDS/build
	cd DIAMONDS/build/
	cmake ..
	make -j 4
	echo "-----------------------------------"
	echo " Compiling and running test demo..."
	echo "-----------------------------------"
	cd ../../
	export LD_LIBRARY_PATH=${PWD}/DIAMONDS/build
	cd DIAMONDS/demos/
	g++ -o demoSingle2DGaussian demoSingle2DGaussian.cpp -L../build/ -I../include/ -ldiamonds -std=c++11
	./demoSingle2DGaussian
	cd ../../
	echo " "
	echo " "	
fi

if [ $flag2 -eq 1 ]; then
	echo "-------------------------------------------"
	echo " Cloning and installing Background code..."
	echo "-------------------------------------------"
	git clone https://github.com/EnricoCorsaro/Background.git
	mkdir Background/build
	cd Background/build/
	cmake ..
	make -j 4
	cd ../../
	echo " "
	echo " "
	echo "-------------------------------------------------------------------------"
	echo " Preparing localPath.txt file with local path of the Background folder..."
	echo "-------------------------------------------------------------------------"
	echo "${PWD}/Background/" > ${PWD}/Background/build/localPath.txt
	echo " "
	echo " "
	echo "--------------------------------------"
	echo " Setting up tutorial for Background..."
	echo "--------------------------------------"
	mkdir Background/data
	mkdir Background/results
	mv Background/tutorials/KIC012008916/KIC012008916.txt Background/data/
	mv Background/tutorials/KIC012008916 Background/results/
	mkdir Background/results/KIC012008916/00
	rm Background/results/KIC012008916/localPath.txt
	echo " "
	echo " "
fi
