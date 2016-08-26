#!/bin/bash
job=$1

cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../dressed_hists

t=1;
for t in `seq 1 1000`
do

    cd ${dir}/${job}/
    hadd main_dressed_${job}_${t}.root RPV_${job}_*_${t}.root
    rm RPV_${job}*_${t}.root    

done

cd ${dir}/../
