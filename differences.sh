#!/bin/sh
# Copyright https://stackoverflow.com/questions/58301988/how-to-calculate-percentage-difference-with-cmp-command
DIFF=`cmp -l $1 $2 | wc -l`
SIZE_A=`wc -c $1 | awk '{print $1}'`
SIZE_B=`wc -c $2 | awk '{print $1}'`
if [ $SIZE_A -gt $SIZE_B ]
then
    MAX=$SIZE_A
else
    MAX=$SIZE_B
fi
echo $DIFF/$MAX*100|bc -l