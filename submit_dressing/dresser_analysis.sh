#!/bin/bash
dress=$1
jobname=$2
j=$3
filename=$4
conf=$5
tmp=$6
PE_number=$7
dir=$8;

scratch_dir=${dir}/scratch/dresser_analysis

date
mkdir ${scratch_dir}
rm -rf ${scratch_dir}/analysis_${jobname}_${j}_${PE_number}
mkdir ${scratch_dir}/analysis_${jobname}_${j}_${PE_number}
cd ${scratch_dir}/analysis_${jobname}_${j}_${PE_number}

shopt -s expand_aliases
source /common/atlas/scripts/setupATLAS.sh
setupATLAS
localSetupROOT
alias root="root -l"

#mkdir files;
#cp ${dir}/submit_dressing/${jobname}_${j}.txt ./files;
cp ${dir}/bin/${dress} dresser
cp ${dir}/config/dress_${conf}.cfg config
echo "templates : " ${dir}/kinematic_hists/${tmp}/main_kinematic_${tmp}.root >> config
echo "PE : " ${PE_number} >> config

./dresser config ${filename}

mkdir ${dir}/dressed_hists/${jobname}/

mv RPV_dress.root ${dir}/dressed_hists/${jobname}/RPV_${jobname}_${j}_${PE_number}.root

cd ;
rm -rf  ${scratch_dir}/analysis_${jobname}_${j}_${PE_number} ;
date
