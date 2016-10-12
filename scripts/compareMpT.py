#!/usr/bin/env python
import ROOT,os,array,pprint, math,time
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()

date ='10_10'
name ='pythia_BDT_PtRatios'
inFile = ROOT.TFile.Open('../samples/'+name+'/main_'+name+'.root')
t = inFile.Get('miniTree')
nEntries = t.GetEntries()
bdtBins = [-1.0,0.0,0.3,1.0]
ptBins = [0.2,0.221,0.244,0.270,0.293,0.329,0.364,0.402,0.445,0.492,0.544,0.6,0.644,0.733,0.811,0.896]
bMatch = [0,1]
nJet = [3,4,5]
nBins=30
xLow=-7
xUp=0
histDict = {}

outFile = ROOT.TFile.Open('output.root','RECREATE')

for ipT in range(len(ptBins)-1):
    for iBDT in range(len(bdtBins)-1):
        for iB in bMatch:
            for iJet in nJet:
                histName = 'h_pT_'+str(ipT)+'_BDT_'+str(iBDT)+'_b_'+str(iB)+'_njet_'+str(iJet)
                histDict[(ipT,iBDT,iB,iJet)] = ROOT.TH1F(histName,histName,nBins,xLow,xUp)
#pprint.pprint(histDict)
#nEntries = 10000
start_time = time.time()
for entry in range(nEntries):
    t.GetEntry(entry)
    if entry % 10000 == 0:
        elapsed_time = time.time() - start_time
        print '----- %s seconds -----' % elapsed_time
        print 'entry = %i' % entry
    ptBin = -1
    bdtBin = -1
    for j in range(len(bdtBins)-1):
        if t.BDTG > bdtBins[j] and t.BDTG < bdtBins[j+1]:
            bdtBin = j
            break
    for i in range(min(4,t.jet_pt.size())):
        if t.jet_pt.at(i) > ptBins[-1]:
            ptBin = len(ptBins)-2
        else:
            for j in range(len(ptBins)-1):
                if t.jet_pt.at(i) > ptBins[j] and t.jet_pt.at(i) < ptBins[j+1]:
                    ptBin = j
                    break
        key = (ptBin,bdtBin,t.jet_bmatched_Fix70.at(i),min(5,t.njet))
        val = -999999
        if t.jet_m.at(i) > 0:
            val = math.log(t.jet_m.at(i)/t.jet_pt.at(i))
        histDict[key].Fill(val,t.weight*t.bSF_70)
        
outFile.Write()
outFile.Close()
