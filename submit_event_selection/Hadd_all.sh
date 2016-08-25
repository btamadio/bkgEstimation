cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../samples

#cd ${dir}/data2015/
#hadd main_kinematic_data2015.root RPV_data2015_*.root

#cd ${dir}/pythia/
#hadd main_pythia.root RPV_pythia_*.root

#cd ${dir}/herwig/
#hadd main_herwig.root RPV_herwig_*.root

cd ${dir}/sherpa_mc15c/
hadd main_sherpa_mc15c.root RPV_sherpa_mc15c_*.root

cd ${dir}/../
