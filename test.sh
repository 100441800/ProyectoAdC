#!/bin/bash
#SBATCH -o ./test.out
#SBATCH -e ./test.out

#./image-soa/imgsoa test/images/in test/images/out copy

#./image-aos/imgaos test/in test/out copy
#cd test/in
#for FILE in *;
#do
#    diff $FILE ../out/$FILE;
#done
#cd ../../
#./image-aos/imgaos test/in test/out histo