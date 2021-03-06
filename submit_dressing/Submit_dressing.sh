cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../

rm *.txt
rm *.sh.e*
rm *.sh.o*

#rm -rf ${dir}/dressed_hists/dataD2
#rm -rf ${dir}/dressed_hists/dataPostD2
#rm -rf ${dir}/dressed_hists/dataDS2
#rm -rf ${dir}/dressed_hists/dataDS2_simple
#rm -rf ${dir}/dressed_hists/dataDS2_sep
#rm -rf ${dir}/dressed_hists/dataDS2_bMatchedTempl
#rm -rf ${dir}/dressed_hists/dataDS2_3jCR
#rm -rf ${dir}/dressed_hists/dataE3
#rm -rf ${dir}/dressed_hists/dataE3_3jCR
#rm -rf ${dir}/dressed_hists/dataE3_simple
#rm -rf ${dir}/dressed_hists/dataE3_sep
#rm -rf ${dir}/dressed_hists/dataE3_bMatched




#rm -rf ${dir}/dressed_hists/ICHEP_Fix60bMatched
#rm -rf ${dir}/dressed_hists/ICHEP_Fix70bMatched
#rm -rf ${dir}/dressed_hists/ICHEP_Fix77bMatched
#rm -rf ${dir}/dressed_hists/ICHEP_Fix85bMatched

#rm -rf ${dir}/dressed_hists/ICHEP_Flt60bMatched
#rm -rf ${dir}/dressed_hists/ICHEP_Flt70bMatched
#rm -rf ${dir}/dressed_hists/ICHEP_Flt77bMatched
#rm -rf ${dir}/dressed_hists/ICHEP_Flt85bMatched

rm -rf ${dir}/dressed_hists/ICHEP_subjetTemplMatchedFix70


#rm -rf ${dir}/dressed_hists/dataE3_10_inv_fb

#rm -rf ${dir}/dressed_hists/pythia
#rm -rf ${dir}/dressed_hists/pythia_3jCR
#rm -rf ${dir}/dressed_hists/pythia_simple
#rm -rf ${dir}/dressed_hists/pythia_sep
#rm -rf ${dir}/dressed_hists/pythia_bMatched

#source submit_analysis.sh dresser proc_dataD2.txt baseline dataD2 ${dir} dataD2
#source submit_analysis.sh dresser_softJetTempl proc_dataDS2.txt softJet dataDS2 ${dir} dataDS2
#source submit_analysis.sh dresser_softJetTempl proc_dataDS2.txt softJet dataDS2 ${dir} dataDS2_simple
#source submit_analysis.sh dresser_softJetTempl proc_dataDS2.txt baseline dataDS2_sep ${dir} dataDS2_sep
#source submit_analysis.sh dresser proc_dataDS2.txt bMatched dataDS2_bMatched ${dir} dataDS2_bMatched
#source submit_analysis.sh dresser proc_dataDS2.txt baseline dataDS2_3jCR ${dir} dataDS2_3jCR

#source submit_analysis.sh dresser_softJetTempl proc_dataE3.txt softJet dataE3 ${dir} dataE3
#source submit_analysis.sh dresser proc_dataE3.txt baseline dataE3_3jCR ${dir} dataE3_3jCR
#source submit_analysis.sh dresser_softJetTempl proc_dataE3.txt softJet_simple dataE3_simple ${dir} dataE3_simple
#source submit_analysis.sh dresser_softJetTempl proc_dataE3.txt baseline dataE3_sep ${dir} dataE3_sep
#source submit_analysis.sh dresser proc_dataE3.txt bMatched dataE3_Flt70bMatched ${dir} dataE3_Flt70bMatched


#source submit_analysis.sh dresser proc_ICHEP.txt bMatchFix60 ICHEP_Fix60bMatched ${dir} ICHEP_Fix60bMatched
#source submit_analysis.sh dresser proc_ICHEP.txt bMatchFix70 ICHEP_Fix70bMatched ${dir} ICHEP_Fix70bMatched
#source submit_analysis.sh dresser proc_ICHEP.txt bMatchFix77 ICHEP_Fix77bMatched ${dir} ICHEP_Fix77bMatched
#source submit_analysis.sh dresser proc_ICHEP.txt bMatchFix85 ICHEP_Fix85bMatched ${dir} ICHEP_Fix85bMatched

#source submit_analysis.sh dresser proc_ICHEP.txt bMatchFlt60 ICHEP_Flt60bMatched ${dir} ICHEP_Flt60bMatched
#source submit_analysis.sh dresser proc_ICHEP.txt bMatchFlt70 ICHEP_Flt70bMatched ${dir} ICHEP_Flt70bMatched
#source submit_analysis.sh dresser proc_ICHEP.txt bMatchFlt77 ICHEP_Flt77bMatched ${dir} ICHEP_Flt77bMatched
#source submit_analysis.sh dresser proc_ICHEP.txt bMatchFlt85 ICHEP_Flt85bMatched ${dir} ICHEP_Flt85bMatched

source submit_analysis.sh dresser_subjetTempl proc_ICHEP44.txt subjetTemplbMatchFix70 ICHEP_subjetTemplbMatchFix70 ${dir} ICHEP_subjetTemplbMatchFix70


#source submit_analysis.sh dresser_softJetTempl proc_data_5_inv_fb.txt softJet data_5_inv_fb ${dir} data_5_inv_fb
#source submit_analysis.sh dresser_softJetTempl proc_data_10_inv_fb.txt softJet data_10_inv_fb ${dir} data_10_inv_fb

#source submit_analysis.sh dresser_softJetTempl proc_Pythia_noJZ2.txt baseline pythia ${dir} pythia_simple
#source submit_analysis.sh dresser proc_Pythia_noJZ2.txt baseline pythia_3jCR ${dir} pythia_3jCR
#source submit_analysis.sh dresser proc_Pythia_noJZ2.txt bMatched pythia_bMatched ${dir} pythia_bMatched

#source submit_analysis.sh dresser_softJetTempl proc_dataPostD2.txt softJet dataPostD2 ${dir} dataPostD2
#source submit_analysis.sh dresser_softJetTempl proc_Pythia_noJZ2.txt softJet pythia ${dir} pythia
#source submit_analysis.sh dresser_softJetTempl proc_sherpa.txt softJet sherpa_softJetTempl ${dir} sherpa_softJetTempl

