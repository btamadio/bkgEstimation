#!/usr/bin/env python
import argparse,ROOT,os,sys,pprint,random
import numpy as np
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
parser = argparse.ArgumentParser(add_help=False, description='Plot Correlation Matrices')
parser.add_argument('input')
args = parser.parse_args()
f = ROOT.TFile.Open(args.input)

xDict = {}
mDict = {}
for njet in [3,4,5]:
    keyStr='n'+str(njet)
    xDict[keyStr]=[]
    mDict[keyStr]=0

print 'Opening file %s' % f.GetName()
t = f.Get('miniTree')
nEntries = t.GetEntries()
print 'Loooping over %i entries.' % nEntries
for entry in range(nEntries):
    t.GetEntry(entry)
    if entry % 10000 == 0:
       print 'entry %i' % entry
    keyString = 'n'+str(min(t.njet,5))
    mDict[keyString]+=1
    indexList = [0,1,2]
        #Randomly assign labels to the 3 or 4 jets:
        # if t.njet == 3:
        #     indexList = [0,1,2]
        #     random.shuffle(indexList)
        #else:
        #    random.shuffle(indexList)
    xDict[keyString].append(t.dEta)
    for i in indexList:
        xDict[keyString].append(t.jet_pt.at(i))
    for i in indexList:
        xDict[keyString].append(t.jet_phi.at(i))
    for i in indexList:
        xDict[keyString].append(t.jet_m.at(i)/t.jet_pt.at(i))
    for i in indexList:
        xDict[keyString].append(t.jet_bmatched_Fix70.at(i))
    xDict[keyString].append(t.MJ)
    xDict[keyString].append(t.nbjet_Fix70)
    xDict[keyString].append(t.HT_ak4)
sigmaDict = {}
xMatDict = {}
histDict = {}
corLabels = [ '|#Delta #eta|','p_{T1}','p_{T2}','p_{T3}',
              '#phi_{1}','#phi_{2}','#phi_{3}',
               'x_{1}','x_{2}','x_{3}',
              'b_{1}','b_{2}','b_{3}',
              'MJ','n_{b-jet}','HT']

outFile = ROOT.TFile.Open('corrMatrices_BDT_FullMJ.root','RECREATE')
outFile.cd()
for key in xDict:
    if mDict[key]>0:
        xMatDict[key]=np.array(xDict[key]).reshape(mDict[key],-1)
        sigmaDict[key] = np.corrcoef(np.transpose(xMatDict[key]))
        nBins = sigmaDict[key].shape[0]
        histDict[key] = ROOT.TH2D('h_'+key,'h_'+key,nBins,-0.5,nBins-0.5,nBins,-0.5,nBins-0.5)
        for i in range(nBins):
            histDict[key].GetXaxis().SetBinLabel(i+1,corLabels[i])
            histDict[key].GetYaxis().SetBinLabel(i+1,corLabels[i])
            for j in range(nBins):
                histDict[key].Fill(i,j,sigmaDict[key][i][j])
outFile.Write()
outFile.Close()
