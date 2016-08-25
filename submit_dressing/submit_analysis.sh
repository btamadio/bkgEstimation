#!/bin/bash
dress=$1;
file=$2;
conf=$3;
tmp=$4;
dir=$5;
jobname=$6;

file=${dir}/files/${file};

j=0;
t=1;
for t in `seq 1 1000`
#for t in `seq 1 100`
do
    while read line
    do  
	qsub -l h_vmem=2G -l projectio=1 dresser_analysis.sh ${dress} ${jobname} $j $line ${conf} ${tmp} $t $dir;
	j=$[1+$j];

    done<${file}
    #echo "finished job submission " $j " jobs submitted. "
done
echo "finished PE submission " $t " PEs submitted. "











