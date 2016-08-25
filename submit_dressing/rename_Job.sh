#!/bin/bash
job=$1

cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../dressed_hists/${job}
cd ${dir};

for t in `seq 1 1000`; do mv RPV_${job}_$[t-1]_${t}.root main_dressed_${job}_${t}.root; done

cd ${dir}/../../
