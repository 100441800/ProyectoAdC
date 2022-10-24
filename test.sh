#!/bin/bash
#SBATCH -o ./test.out
#SBATCH -e ./test.out

#./image-soa/imgsoa test/images/in test/images/out copy
mkdir -p ./test/in
mkdir -p ./test/out

cp ./test/images/* ./test/in/
./image-aos/imgaos test/in test/out copy

cd ./test/in

for FILE in *;
do
    diff $FILE ../out/$FILE;
done
cd ../../
rm -rf ./test/in/*
rm -rf ./test/out/*