cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../dressed_hists


t=1;
for t in `seq 1 100`
do
    #cd ${dir}/pythia/
    #hadd main_dressed_pythia_${t}.root RPV_pythia_*_${t}.root
    #rm RPV_pythia_*_${t}.root

    cd ${dir}/pythia_noEtaCut/
    hadd main_dressed_pythia_noEtaCut_${t}.root RPV_pythia_noEtaCut_*_${t}.root
    rm RPV_pythia_noEtaCut*_${t}.root    

    #cd ${dir}/data2015/
    #hadd main_dressed_data2015_${t}.root RPV_data2015_*_${t}.root
    #rm RPV_data2015_*_${t}.root    

    #cd ${dir}/herwig/
    #hadd main_dressed_herwig_${t}.root RPV_herwig_*_${t}.root
    #rm RPV_herwig_*_${t}.root    

    #cd ${dir}/sherpa/
    #hadd main_dressed_sherpa_${t}.root RPV_sherpa_*_${t}.root
    #rm RPV_sherpa_*_${t}.root    
done

cd ${dir}/../
