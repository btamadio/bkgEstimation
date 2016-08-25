#!/bin/bash
job=$1

cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../kinematic_hists

cd ${dir}/${job}/
hadd main_kinematic_${job}.root RPV_${job}_*.root 

cd ${dir}/../
