#!/usr/bin/env python
import argparse,ROOT,os,glob,sys,subprocess
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
ROOT.gROOT.SetBatch(True)
fileNameList = ['../samples/pythia/main_pythia.root',
                '../samples/RPV10_MCInject_403577/RPV_RPV10_MCInject_403577_17.root',
                '../samples/ICHEP44/main_ICHEP44.root']
fileList = [ ROOT.TFile.Open(fileName) for fileName in fileNameList ]
colorList = [ROOT.kBlue, ROOT.kRed, ROOT.kBlack]
mjCutList = ['MJ > 0.8','MJ > 0.8','MJ > 0.6','MJ > 0.6']
njetCutList = ['njet == 4','njet == 4','njet >= 5','njet >= 5']
bjetCutList = ['nbjet_Fix70 == 0','nbjet_Fix70 > 0','nbjet_Fix70 == 0','nbjet_Fix70 > 0']
drawOptList = ['hist','hist same','pe same']
c=[]
histList = [[],[],[],[]]
for i in range(4):
    c.append(ROOT.TCanvas('c_'+str(i),'c_'+str(i),800,600))
    c[i].cd()
    selectStr='weight*('+mjCutList[i]+' && '+njetCutList[i]+' && dEta < 1.4 && '+bjetCutList[i]+')'
    for j in range(len(fileList)):
        histList[i].append(ROOT.TH1F('h_'+str(i)+'_'+str(j),'h_'+str(i)+'_'+str(j),8,0.5,8.5))
        histList[i][j].Sumw2()
        drawString = 'jet_NTrimSubjets>>h_'+str(i)+'_'+str(j)
        fileList[j].Get('miniTree').Draw(drawString,selectStr)
        histList[i][j].Scale(1./histList[i][j].Integral())
        histList[i][j].SetLineColor(colorList[j])
srLabelList = ['4jSRb0','4jSRb1','5jSRb0','5jSRb1']
lat = ROOT.TLatex()
leg1 = ROOT.TLegend(.65,.3,.85,.65)
leg1.SetBorderSize(0)
leg1.SetFillStyle(0)
leg1.SetTextSize(0.04)
leg1.AddEntry(histList[0][0],'Pythia','F')
leg1.AddEntry(histList[0][1],'#splitline{m_{#tilde{g}} = 1.6 TeV}{m_{#tilde{#chi}_{1}^{0}} = 1.25 TeV}','F')
leg1.AddEntry(histList[0][2],'data')

for i in range(4):
    c[i].cd()
    for j in range(3):
        histList[i][j].Draw(drawOptList[j])
        histList[i][j].GetXaxis().SetTitle('NTrimSubjets')
        histList[i][j].GetYaxis().SetTitle('Fraction of jets')
        histList[i][j].GetYaxis().SetTitleOffset(1.4)
    ROOT.ATLASLabel(0.65,0.87,'Internal')
    lat.DrawLatexNDC(.65,.8,srLabelList[i])
    lat.DrawLatexNDC(.65,.7,'#int L dt = 12.7 fb^{-1}')
    leg1.Draw()
    outFileName = '/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_16_NTrimSubjets/NTrimSubjets_'+srLabelList[i]
    c[i].Print(outFileName+'.pdf')
    c[i].Print(outFileName+'.png')
    os.system('chmod a+r /project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_16_NTrimSubjets/*')

