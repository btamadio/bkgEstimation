#!/usr/bin/env python
import argparse,ROOT,os,sys
import numpy as np
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
#ROOT.gROOT.SetBatch(True)
parser = argparse.ArgumentParser(add_help=False, description='Plot Correlation Matrices')
parser.add_argument('input')
args = parser.parse_args()
njet = 3
f = ROOT.TFile.Open(args.input)
print 'Opening file %s' % f.GetName()
t = f.Get('miniTree')
print 'Loooping over %i entries.' % t.GetEntries()
xList = []
m = 0
for entry in range(t.GetEntries()):
    t.GetEntry(entry)
    if entry % 10000 == 0:
        print 'entry %i' % entry
    if t.njet == njet:
        m+=1
        for i in range(len(t.jet_pt)):
            xList.append(t.jet_pt.at(i))
        for i in range(len(t.jet_eta)):
            xList.append(abs(t.jet_eta.at(i)))
x=np.array(xList).reshape(m,-1)
nBins = 20
Sigma = np.corrcoef(np.transpose(x))
U,s,V = np.linalg.svd(Sigma)


# corMatrix = ROOT.TH2D('h','h',nBins,-0.5,nBins-0.5,nBins,-0.5,nBins-0.5)
# corLabels = ['x_{1}','x_{2}','x_{3}','x_{4}',
#              'p_{T1}','p_{T2}','p_{T3}','p_{T4}',
#              '|#eta_{1}|','|#eta_{2}|','|#eta_{3}|','|#eta_{4}|',
#              'n_{1}','n_{2}','n_{3}','n_{4}',
#              'b_{1}','b_{2}','b_{3}','b_{4}']

# for i in range(nBins):
#     corMatrix.GetXaxis().SetBinLabel(i+1,corLabels[i])
#     corMatrix.GetYaxis().SetBinLabel(i+1,corLabels[i])
#     corMatrix.GetXaxis().SetTickLength(0)
#     corMatrix.GetYaxis().SetTickLength(0)
#     for j in range(nBins):
#         corMatrix.Fill(i,j,Sigma[i][j])

# c = ROOT.TCanvas('c','c',800,600)
# ROOT.gStyle.SetPaintTextFormat('2.2f')
# corMatrix.Draw('col0 text')
