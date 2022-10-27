#!/bin/bash
#SBATCH -o ./test.out
#SBATCH -e ./test.out

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
BOLD='\033[1;37m'

mkdir -p ./test/in
mkdir -p ./test/out

cp ./test/images/* ./test/in/

# AOS
echo -e "\nGoing to test ${BOLD}imgaos${NC}:"
rm -rf ./test/out/*

./image-aos/imgaos test/in test/out copy > /dev/null

cd ./test/in
# Clean

for FILE in *;
do
    echo -n -e "\tCopy of ${FILE} " 
    if (diff $FILE ../out/$FILE > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../
# Clean
rm -rf ./test/out/*

./image-aos/imgaos test/in test/out histo > /dev/null

cd ./test/histo

for FILE in *;
do
    echo -n -e "\tHistogram of ${FILE} " 
    
    if (diff $FILE ../out/${filename} > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../
rm -rf ./test/out/*

./image-aos/imgaos test/in test/out mono > /dev/null

cd ./test/mono
# Clean

for FILE in *;
do
    echo -n -e "\tMono of ${FILE} " 
    if (diff $FILE ../out/$FILE > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../
# Clean
rm -rf ./test/out/*

./image-aos/imgaos test/in test/out gauss > /dev/null

cd ./test/gauss

for FILE in *;
do
    echo -n -e "\tGauss of ${FILE} " 
    if (diff $FILE ../out/$FILE > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../
# Clean
rm -rf ./test/out/*

# SOA
echo -e "\nGoing to test ${BOLD}imgsoa${NC}:"

./image-soa/imgsoa test/in test/out copy > /dev/null

cd ./test/in

for FILE in *;
do
    echo -n -e "\tCopy of ${FILE} " 
    if (diff $FILE ../out/$FILE > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
done
cd ../../
# Clean
rm -rf ./test/out/*

./image-aos/imgaos test/in test/out histo > /dev/null

cd ./test/histo

for FILE in *;
do
    echo -n -e "\tHistogram of ${FILE} " 
    filename=$(basename ${FILE})

    if (diff $FILE ../out/${filename} > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../
# Clean
rm -rf ./test/out/*

./image-aos/imgaos test/in test/out mono > /dev/null

cd ./test/mono

for FILE in *;
do
    echo -n -e "\tMono of ${FILE} " 
    if (diff $FILE ../out/$FILE > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../
# Clean
rm -rf ./test/out/*

./image-aos/imgaos test/in test/out gauss > /dev/null

cd ./test/gauss

for FILE in *;
do
    echo -n -e "\tGauss of ${FILE} " 
    if (diff $FILE ../out/$FILE > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../

# Clean
rm -rf ./test/out/*