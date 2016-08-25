#!/bin/bash
kin=$1;
jobname=$2
j=$3
filename=$4
conf=$5
dir=$6

scratch_dir=${dir}/scratch/kinematic_analyis/

date
mkdir ${scratch_dir}
rm -rf ${scratch_dir}/analysis_${jobname}_${j}
mkdir ${scratch_dir}/analysis_${jobname}_${j}
cd ${scratch_dir}/analysis_${jobname}_${j}

shopt -s expand_aliases
source /common/atlas/scripts/setupATLAS.sh
setupATLAS
localSetupROOT
alias root="root -l"

#mkdir samples;
#cp ${filename} ./samples;
cp ${dir}/bin/${kin} kinematicMaker

./kinematicMaker ${dir}/config/kin_${conf}.cfg ${filename}

mkdir ${dir}/kinematic_hists/${jobname}/

mv RPV_kin.root ${dir}/kinematic_hists/${jobname}/RPV_${jobname}_${j}.root

cd ;
rm -rf  ${scratch_dir}/analysis_${jobname}_${j} ;
date
