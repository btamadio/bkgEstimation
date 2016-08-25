cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../kinematic_hists

#cd ${dir}/data2015/
#hadd main_kinematic_data2015.root RPV_data2015_*.root

#cd ${dir}/pythia/
#hadd main_kinematic_pythia.root RPV_pythia_*.root

#cd ${dir}/pythia_noEtaCut/
#hadd main_kinematic_pythia_noEtaCut.root RPV_pythia_noEtaCut*.root

cd ${dir}/pythia_splitTemp/
hadd main_kinematic_pythia_splitTemp.root RPV_pythia_splitTemp*.root

#cd ${dir}/herwig/
#hadd main_kinematic_herwig.root RPV_herwig_*.root

#cd ${dir}/sherpa/
#hadd main_kinematic_sherpa.root RPV_sherpa_*.root

cd ${dir}/../
