#!/bin/bash
kin=$1;
file=$2;
conf=$3;
dir=$4;
jobname=$5;

file=${dir}/files/${file};

j=0
while read line
do
	qsub -l h_vmem=1G -l projectio=1 kinematic_analysis.sh ${kin} ${jobname} $j $line ${conf} ${dir};
	j=$[1+$j];	

done<${file}
echo "finished job submission " $j " jobs submitted. "











