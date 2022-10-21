#!/bin/bash
#SBATCH -o ./test.out
#SBATCH -e ./test.out

#./image-soa/imgsoa test/images/in test/images/out copy
./image-aos/imgaos test/images/in test/images/out copy
