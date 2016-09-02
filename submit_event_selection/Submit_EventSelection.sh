cdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
dir=${cdir}/../

echo ${dir}

#rm *.txt
#rm *.sh.e*
#rm *.sh.o*

#rm -rf ${dir}/samples/data2015
rm -rf ${dir}/samples/data2016
#rm -rf ${dir}/samples/pythia
#rm -rf ${dir}/samples/sherpa
#rm -rf ${dir}/samples/rpv10_mg1200_mx50
#rm -rf ${dir}/samples/rpv10_mg1600_mx650
#rm -rf ${dir}/samples/rpv6_mg1200

#source submit_selection.sh /project/projectdirs/atlas/btamadio/RPV_SUSY/NTUP/fileList.data15_13TeV.physics_Main.2016_06_14_v1.txt data2015 ${dir}

#source submit_selection.sh /project/projectdirs/atlas/btamadio/RPV_SUSY/NTUP/fileList.data16_13TeV.physics_Main.2016_06_14_v1.txt data2016 ${dir}

source submit_selection.sh ${dir}/files/fileList.data16_13TeV.physics_Main.2016_07_27_v2.txt data2016 ${dir} #ICHEP


#source submit_selection.sh ${dir}/files/fileList.data15_13TeV.physics_Main.2016_06_14_v1.txt data2015 ${dir}
#source submit_selection.sh ${dir}/files/fileList.data16_13TeV.physics_Main.2016_07_06_v1.txt data2016 ${dir} #D2
#source submit_selection.sh ${dir}/files/fileList.data16_13TeV.postD2.07_22_v1.txt data2016_postD2 ${dir} #postD2
#source submit_selection.sh ${dir}/files/fileList.data16_13TeV.physics_Main.for_5_inverse_fb.txt data2016_5_inv_fb ${dir}
#source submit_selection.sh ${dir}/files/fileList.data16_13TeV.physics_Main.for_10_inverse_fb.txt data2016_10_inv_fb ${dir}
#source submit_selection.sh ${dir}/files/fileList.data15_13TeV.debugrec_hlt.2016_07_25_v1.txt data2015_debug ${dir} 
#source submit_selection.sh ${dir}/files/fileList.data16_13TeV.debugrec_hlt.2016_07_25_v1.txt data2016_debug ${dir}
#source submit_selection.sh ${dir}/files/fileList.data16_13TeV.physics_Main.2016_07_27_v2.txt data2016 ${dir} #ICHEP
#source submit_selection.sh ${dir}/files/fileList.mc15_13TeV.PythiaMultijets.2016_07_25_v1.txt pythia ${dir}
#source submit_selection.sh ${dir}/files/fileList.mc15_13TeV.SherpaMultijets.2016_07_15_v1.txt sherpa ${dir}
#source submit_selection.sh ${dir}/files/rpv10_mg1200_mx50.txt rpv10_mg1200_mx50 ${dir}
#source submit_selection.sh ${dir}/files/rpv10_mg1600_mx650.txt rpv10_mg1600_mx650 ${dir}
#source submit_selection.sh ${dir}/files/rpv6_mg1200.txt rpv6_mg1200 ${dir}
#source submit_selection.sh ${dir}/files/fileList.mc15_13TeV.ttH.2016_06_30_v1.txt ttH ${dir}
