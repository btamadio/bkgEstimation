#!/bin/bash
file=$1
j=0
while read line
do
j=$[1+$j];
k=$[$j/100];
echo $line >> RPV_pythia_${k}.txt;
done<${file}
