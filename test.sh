#!/bin/bash
#SBATCH -o ./test.out
#SBATCH -e ./test.out

!/bin/bash
./image-soa/imgsoa a b 
./image-aos/imgaos a b 
