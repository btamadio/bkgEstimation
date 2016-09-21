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
def getKeyStringList(njet,nbjet,mj,dEta):
    #there are 8 keys for each event, because each of 3 observables can be exclusive or inclusive:
    #MJ, b-tag, and dEta
    keyStringList = ['n'+str(min(5,njet))+'_b9_MJ_00_130_dEta_00_40']
    keyStringList.append('n'+str(min(5,njet))+'_b'+str(int((nbjet>0)))+'_MJ_00_130_dEta_00_40')
    if mj < 0.2:
        mjStr = '00_02'
    elif mj >= 0.2 and mj < 0.6:
        mjStr = '02_06'
    else:
        mjStr = '06_130'
    if dEta < 1.4:
        dEtaStr = '00_14'
    else:
        dEtaStr = '14_40'

    keyStringList.append('n'+str(min(5,njet))+'_b9_MJ_'+mjStr+'_dEta_00_40')
    keyStringList.append('n'+str(min(5,njet))+'_b'+str(int((nbjet>0)))+'_MJ_'+mjStr+'_dEta_00_40')

    keyStringList.append('n'+str(min(5,njet))+'_b9_MJ_00_130_dEta_'+dEtaStr)
    keyStringList.append('n'+str(min(5,njet))+'_b'+str(int((nbjet>0)))+'_MJ_00_130_dEta_'+dEtaStr)
    keyStringList.append('n'+str(min(5,njet))+'_b9_MJ_'+mjStr+'_dEta_'+dEtaStr)
    keyStringList.append('n'+str(min(5,njet))+'_b'+str(int((nbjet>0)))+'_MJ_'+mjStr+'_dEta_'+dEtaStr)
    return keyStringList
    #pprint.pprint(keyStringList)

mjStr = ['00_02','02_06','06_130','00_130']
dEtaStr = ['00_14','14_40','00_40']
xDict = {}
mDict = {}
for njet in [3,4,5]:
    for btag in [0,1,9]:
        for mj in [0,1,2,3]:
            for dEta in [0,1,2]:
                keyStr='n'+str(njet)+'_b'+str(btag)+'_MJ_'+mjStr[mj]+'_dEta_'+dEtaStr[dEta]
                xDict[keyStr]=[]
                mDict[keyStr]=0

print 'Opening file %s' % f.GetName()
t = f.Get('miniTree')
nEntries = t.GetEntries()
#nEntries=10000

print 'Loooping over %i entries.' % nEntries
for entry in range(nEntries):
    t.GetEntry(entry)
    if entry % 10000 == 0:
       print 'entry %i' % entry
    keyStringList = getKeyStringList(t.njet,t.nbjet_Fix70,t.MJ,t.dEta)
    for keyString in keyStringList:
        mDict[keyString]+=1
        indexList = [0,1,2]
        random.shuffle(indexList)
        for i in indexList:
            xDict[keyString].append(t.jet_pt.at(i))
        for i in indexList:
            xDict[keyString].append(t.jet_eta.at(i))
        for i in indexList:
            xDict[keyString].append(t.jet_m.at(i)/t.jet_pt.at(i))
        for i in indexList:
            xDict[keyString].append(t.jet_NTrimSubjets.at(i))
        for i in indexList:
            xDict[keyString].append(t.jet_bmatched_Fix70.at(i))
        xDict[keyString].append(t.MJ)
        xDict[keyString].append(t.nbjet_Fix70)

sigmaDict = {}
xMatDict = {}
histDict = {}
corLabels = ['p_{T1}','p_{T2}','p_{T3}',
             '#eta_{1}','#eta_{2}','#eta_{3}',
              'x_{1}','x_{2}','x_{3}',
             'n_{1}','n_{2}','n_{3}',
             'b_{1}','b_{2}','b_{3}',
             'MJ','nbjet']

outFile = ROOT.TFile.Open('output.root','RECREATE')
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
