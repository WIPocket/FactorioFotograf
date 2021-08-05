#!/bin/sh
cd src
./fotograf $@
rm ../$1 -rf
mv $1 ../
cd ..

echo "Output is in $(pwd)/$1"
