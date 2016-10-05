#!/usr/bin/env python
import ROOT,os,array
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
ROOT.gStyle.SetPaintTextFormat('2.1f')
#ROOT.gROOT.SetBatch(True)
sigFile = ROOT.TFile.Open('../samples/RPV10_MCInject_403577/RPV_RPV10_MCInject_403577_17.root')
bkgFile = ROOT.TFile.Open('../samples/pythia/main_pythia.root')
subjetBins = [0.5,1.5,2.5,3.5,4.5]
ptBins = [0.2,0.221,0.244,0.270,0.293,0.329,0.364,0.402,0.445,0.492,0.544,0.6,0.644,0.733,0.811,0.896]
outFile = ROOT.TFile.Open('output.root','RECREATE')
bkgHist_b0 = ROOT.TH2F('bkgHist_b0','bkgHist_b0',len(ptBins)-1,array.array('d',ptBins),len(subjetBins)-1,array.array('d',subjetBins))
sigHist_b0 = ROOT.TH2F('sigHist_b0','sigHist_b0',len(ptBins)-1,array.array('d',ptBins),len(subjetBins)-1,array.array('d',subjetBins))

bkgHist_b1 = ROOT.TH2F('bkgHist_b1','bkgHist_b1',len(ptBins)-1,array.array('d',ptBins),len(subjetBins)-1,array.array('d',subjetBins))
sigHist_b1 = ROOT.TH2F('sigHist_b1','sigHist_b1',len(ptBins)-1,array.array('d',ptBins),len(subjetBins)-1,array.array('d',subjetBins))
ratHist_b1 = ROOT.TH2F('ratHist_b1','ratHist_b1',len(ptBins)-1,array.array('d',ptBins),len(subjetBins)-1,array.array('d',subjetBins))

lumi = 14.77
bkgTree = bkgFile.Get('miniTree')
print 'Looping over %i entries.' % bkgTree.GetEntries()
for entry in range(bkgTree.GetEntries()):
    bkgTree.GetEntry(entry)
    if entry % 1E5 == 0:
        print 'entry %i' % entry
    if bkgTree.njet == 3:
        for i in range(len(bkgTree.jet_pt)):
            if bkgTree.jet_bmatched_Fix70.at(i) == 0:
                bkgHist_b0.Fill(bkgTree.jet_pt.at(i),min(4,bkgTree.jet_NTrimSubjets.at(i)),bkgTree.weight*lumi)
            else:
                bkgHist_b1.Fill(bkgTree.jet_pt.at(i),min(4,bkgTree.jet_NTrimSubjets.at(i)),bkgTree.weight*lumi)

sigTree = sigFile.Get('miniTree')
print 'Looping over %i entries.' % sigTree.GetEntries()
for entry in range(sigTree.GetEntries()):
    sigTree.GetEntry(entry)
    if sigTree.njet == 3:
        for i in range(len(sigTree.jet_pt)):
            if sigTree.jet_bmatched_Fix70.at(i) == 0:
                sigHist_b0.Fill(sigTree.jet_pt.at(i),min(4,sigTree.jet_NTrimSubjets.at(i)),sigTree.weight*lumi)
            else:
                sigHist_b1.Fill(sigTree.jet_pt.at(i),min(4,sigTree.jet_NTrimSubjets.at(i)),sigTree.weight*lumi)

ratHist_b0 = sigHist_b0.Clone('ratHist_b0')
ratHist_b1 = sigHist_b1.Clone('ratHist_b1')

ratHist_b0.Divide(bkgHist_b0)
ratHist_b1.Divide(bkgHist_b1)
outFile.Write()
