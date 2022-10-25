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

./image-aos/imgaos test/in test/out copy > /dev/null

cd ./test/in
# Clean
rm -rf ./test/out/*

for FILE in *;
do
    echo -n -e "\tCopy of ${FILE} " 
    if (diff $FILE ../copy/$FILE > /dev/null)
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

cd ./test/in

for FILE in *;
do
    echo -n -e "\tHistogram of ${FILE} " 
    filename=$(basename ${FILE})
    filename="${filename/.bmp/}"
    
    if (diff $FILE ../histo/${filename}.hst > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../

./image-aos/imgaos test/in test/out mono > /dev/null

cd ./test/in
# Clean
rm -rf ./test/out/*

for FILE in *;
do
    echo -n -e "\tMono of ${FILE} " 
    if (diff $FILE ../mono/$FILE > /dev/null)
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

cd ./test/in

for FILE in *;
do
    echo -n -e "\tGauss of ${FILE} " 
    if (diff $FILE ../gauss/$FILE > /dev/null)
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
    if (diff $FILE ../copy/$FILE)
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

cd ./test/in

for FILE in *;
do
    echo -n -e "\tHistogram of ${FILE} " 
    filename=$(basename ${FILE})
    filename="${filename/.bmp/}"
    
    if (diff $FILE ../histo/${filename}.hst > /dev/null)
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

cd ./test/in

for FILE in *;
do
    echo -n -e "\tMono of ${FILE} " 
    if (diff $FILE ../mono/$FILE > /dev/null)
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

cd ./test/in

for FILE in *;
do
    echo -n -e "\tGauss of ${FILE} " 
    if (diff $FILE ../gauss/$FILE > /dev/null)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} ✗${NC}"
    fi
    
done
cd ../../

# Clean
rm -rf ./test/out/*