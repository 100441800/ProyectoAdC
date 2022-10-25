#!/bin/bash
#SBATCH -o ./test.out
#SBATCH -e ./test.out

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
BOLD='\033[1;37m'


# AOS
mkdir -p ./test/in
mkdir -p ./test/out

cp ./test/images/* ./test/in/
echo -e "\nGoing to test ${BOLD}imgaos${NC}:"
./image-aos/imgaos test/in test/out copy > /dev/null

cd ./test/in

for FILE in *;
do
    echo -n -e "Copy of ${FILE} " 
    if (diff $FILE ../out/$FILE)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} Error${NC}"
    fi
done
cd ../../

# Clean
rm -rf ./test/in/*
rm -rf ./test/out/*

# SOA

cp ./test/images/* ./test/in/
echo -e "\nGoing to test ${BOLD}imgsoa${NC}:"
./image-soa/imgsoa test/in test/out copy > /dev/null

cd ./test/in

for FILE in *;
do
    echo -n -e "Copy of ${FILE} " 
    if (diff $FILE ../out/$FILE)
    then
        echo -e "${GREEN} ✓${NC}"
    else
        echo -e "${RED} Error${NC}"
    fi
done
cd ../../

# Clean
rm -rf ./test/in/*
rm -rf ./test/out/*