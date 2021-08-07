#!/bin/sh

cd $(dirname "$0")

cd src               &&
./fotograf.sh $@     &&
command rm ../$1 -rf &&
mv $1 ../            &&
cd ..                &&

echo "Output is in $(pwd)/$1"
