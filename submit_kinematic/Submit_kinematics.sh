cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../

rm *.txt
rm *.sh.e*
rm *.sh.o*

#rm -rf ${dir}/kinematic_hists/dataD2
#rm -rf ${dir}/kinematic_hists/dataPostD2
#rm -rf ${dir}/kinematic_hists/dataDS2
#rm -rf ${dir}/kinematic_hists/dataDS2_sep
#rm -rf ${dir}/kinematic_hists/dataDS2
#rm -rf ${dir}/kinematic_hists/dataDS2_3jCR

#rm -rf ${dir}/kinematic_hists/dataE3
#rm -rf ${dir}/kinematic_hists/dataE3_3jCR
#rm -rf ${dir}/kinematic_hists/dataE3_sep
#rm -rf ${dir}/kinematic_hists/dataE3_simple
#rm -rf ${dir}/kinematic_hists/dataE3_bMatched
#rm -rf ${dir}/kinematic_hists/dataE3_Flt70bMatched

#rm -rf ${dir}/kinematic_hists/ICHEP_Fix60bMatched
#rm -rf ${dir}/kinematic_hists/ICHEP_Fix70bMatched
#rm -rf ${dir}/kinematic_hists/ICHEP_Fix77bMatched
#rm -rf ${dir}/kinematic_hists/ICHEP_Fix85bMatched
#rm -rf ${dir}/kinematic_hists/ICHEP_Flt60bMatched
#rm -rf ${dir}/kinematic_hists/ICHEP_Flt70bMatched
#rm -rf ${dir}/kinematic_hists/ICHEP_Flt77bMatched
#rm -rf ${dir}/kinematic_hists/ICHEP_Flt85bMatched

rm -rf ${dir}/kinematic_hists/ICHEP_subjetTemplbMatchFix70

#rm -rf ${dir}/kinematic_hists/data_5inv_fb
#rm -rf ${dir}/kinematic_hists/data_10inv_fb

#rm -rf ${dir}/kinematic_hists/sherpa
#rm -rf ${dir}/kinematic_hists/pythia
#rm -rf ${dir}/kinematic_hists/pythia_3jCR
#rm -rf ${dir}/kinematic_hists/pythia_simple
#rm -rf ${dir}/kinematic_hists/pythia_sep
#rm -rf ${dir}/kinematic_hists/pythia_bMatched

#source submit_analysis.sh executable fileList configFile ${dir} jobname

#source submit_analysis.sh kinematicMaker_softJetTempl proc_dataD2.txt softJetTempl ${dir} dataD2
#source submit_analysis.sh kinematicMaker_softJetTempl proc_dataPostD2.txt softJetTempl ${dir} dataPostD2
#source submit_analysis.sh kinematicMaker_softJetTempl proc_dataDS2.txt softJetTempl ${dir} dataDS2
#source submit_analysis.sh kinematicMaker proc_dataDS2.txt bMatchedTempl ${dir} dataDS2_bMatched
#source submit_analysis.sh kinematicMaker proc_dataDS2.txt coarseGrid ${dir} dataDS2_3jCR

#source submit_analysis.sh kinematicMaker_softJetTempl proc_dataE3.txt softJetTempl ${dir} dataE3
#source submit_analysis.sh kinematicMaker_softJetTempl proc_dataE3.txt softJetTempl ${dir} dataE3_sep
#source submit_analysis.sh kinematicMaker_softJetTempl proc_dataE3.txt softJetTempl ${dir} dataE3_simple
#source submit_analysis.sh kinematicMaker proc_dataE3.txt baseline ${dir} dataE3_3jCR
#source submit_analysis.sh kinematicMaker proc_dataE3.txt bMatchedTempl ${dir} dataE3_bMatched

#source submit_analysis.sh kinematicMaker proc_ICHEP.txt bMatchFix60 ${dir} ICHEP_Fix60bMatched
#source submit_analysis.sh kinematicMaker proc_ICHEP.txt bMatchFix70 ${dir} ICHEP_Fix70bMatched
#source submit_analysis.sh kinematicMaker proc_ICHEP.txt bMatchFix77 ${dir} ICHEP_Fix77bMatched
#source submit_analysis.sh kinematicMaker proc_ICHEP.txt bMatchFix85 ${dir} ICHEP_Fix85bMatched

#source submit_analysis.sh kinematicMaker proc_ICHEP.txt bMatchFlt60 ${dir} ICHEP_Flt60bMatched
#source submit_analysis.sh kinematicMaker proc_ICHEP.txt bMatchFlt70 ${dir} ICHEP_Flt70bMatched
#source submit_analysis.sh kinematicMaker proc_ICHEP.txt bMatchFlt77 ${dir} ICHEP_Flt77bMatched
#source submit_analysis.sh kinematicMaker proc_ICHEP.txt bMatchFlt85 ${dir} ICHEP_Flt85bMatched

source submit_analysis.sh kinematicMaker_subjetTempl proc_ICHEP44.txt subjetTemplbMatchFix70 ${dir} ICHEP_subjetTemplbMatchFix70

#source submit_analysis.sh kinematicMaker_softJetTempl proc_data_5_inv_fb.txt softJetTempl ${dir} data_5_inv_fb
#source submit_analysis.sh kinematicMaker_softJetTempl proc_data_10_inv_fb.txt softJetTempl ${dir} data_10_inv_fb

#source submit_analysis.sh kinematicMaker proc_Pythia_noJZ2.txt baseline ${dir} pythia_3jCR
#source submit_analysis.sh kinematicMaker proc_Pythia_noJZ2.txt bMatchedTempl ${dir} pythia_bMatched

#source submit_analysis.sh kinematicMaker_softJetTempl proc_sherpa.txt softJetTempl ${dir} sherpa
#source submit_analysis.sh kinematicMaker_softJetTempl proc_Pythia_noJZ2.txt softJetTempl ${dir} pythia
#source submit_analysis.sh kinematicMaker_softJetTempl proc_mg1200.txt softJetTempl ${dir} mg1200
#source submit_analysis.sh kinematicMaker_softJetTempl proc_mg1200_mx50.txt softJetTempl ${dir} mg1200_mx50
#source submit_analysis.sh kinematicMaker_softJetTempl proc_mg1600_mx650.txt softJetTempl ${dir} mg1600_mx650
