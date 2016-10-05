#!/usr/bin/env python
import argparse,ROOT,os,glob,sys,subprocess,array
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
ROOT.gStyle.SetPaintTextFormat('2.2f')
#ROOT.gROOT.SetBatch(True)
parser = argparse.ArgumentParser(add_help=False, description='Plot N_subjets')
parser.add_argument('input')
args = parser.parse_args()
filePath = args.input.strip('/')
print filePath

inFile = ROOT.TFile.Open(filePath)
miniTree = inFile.Get('miniTree')

etaBins = [0,0.5,1.0,1.5,2.0]
ptBins = [0.2,0.221,0.244,0.270,0.293,0.329,0.364,0.402,0.445,0.492,0.544,0.6,0.644,0.733,0.811,0.896]

h3_b0 = ROOT.TH2F('n3_b0','h3_b0',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))
h4_b0 = ROOT.TH2F('ratio_n4_n3_b0','h4_b0',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))
h5_b0 = ROOT.TH2F('ratio_n5_n3_b0','h5_b0',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))

h3_b1 = ROOT.TH2F('n3_b1','h3_b1',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))
h4_b1 = ROOT.TH2F('ratio_n4_n3_b1','h4_b1',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))
h5_b1 = ROOT.TH2F('ratio_n5_n3_b1','h5_b1',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))

h3_b0_n = ROOT.TH2F('n3_b0_n','h3_b0_n',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))
h4_b0_n = ROOT.TH2F('n4_b0_n','h4_b0_n',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))
h5_b0_n = ROOT.TH2F('n5_b0_n','h5_b0_n',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))

h3_b1_n = ROOT.TH2F('n3_b1_n','h3_b1_n',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))
h4_b1_n = ROOT.TH2F('n4_b1_n','h4_b1_n',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))
h5_b1_n = ROOT.TH2F('n5_b1_n','h5_b1_n',len(ptBins)-1,array.array('d',ptBins),len(etaBins)-1,array.array('d',etaBins))

for entry in range(miniTree.GetEntries()):
    miniTree.GetEntry(entry)
    for i in range(len(miniTree.jet_pt)):
        if miniTree.njet == 3:
            if miniTree.jet_bmatched_Fix70.at(i) == 0:
                h3_b0.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), miniTree.jet_NTrimSubjets.at(i))
                h3_b0_n.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), 1)
            elif miniTree.jet_bmatched_Fix70.at(i) == 1:
                h3_b1.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), miniTree.jet_NTrimSubjets.at(i))
                h3_b1_n.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), 1)
        elif miniTree.njet >= 4:
            if miniTree.jet_bmatched_Fix70.at(i) == 0:
                h4_b0.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), miniTree.jet_NTrimSubjets.at(i))
                h4_b0_n.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), 1)
            elif miniTree.jet_bmatched_Fix70.at(i) == 1:
                h4_b1.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), miniTree.jet_NTrimSubjets.at(i))
                h4_b1_n.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), 1)
        if miniTree.njet >= 5:
            if miniTree.jet_bmatched_Fix70.at(i) == 0:
                h5_b0.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), miniTree.jet_NTrimSubjets.at(i))
                h5_b0_n.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), 1)
            elif miniTree.jet_bmatched_Fix70.at(i) == 1:
                h5_b1.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), miniTree.jet_NTrimSubjets.at(i))
                h5_b1_n.Fill( min(miniTree.jet_pt.at(i),0.85), abs(miniTree.jet_eta.at(i)), 1)

h3_b0.Divide(h3_b0_n)
h4_b0.Divide(h4_b0_n)
h5_b0.Divide(h5_b0_n)
h3_b1.Divide(h3_b1_n)
h4_b1.Divide(h4_b1_n)
h5_b1.Divide(h5_b1_n)

h4_b0.Divide(h3_b0)
h5_b0.Divide(h3_b0)
h4_b1.Divide(h3_b1)
h5_b1.Divide(h3_b1)

histList = [h3_b0,h4_b0,h5_b0,h3_b1,h4_b1,h5_b1]
c=[]
for hist in histList:
    hist.GetXaxis().SetTitle('jet p_{T} [GeV]')
    hist.GetYaxis().SetTitle('jet |#eta|')
    hist.SetMarkerSize(2)
    c.append(ROOT.TCanvas('c_'+str(len(c)),'c_'+str(len(c)),800,600))
    c[len(c)-1].cd()
    hist.Draw('text45')
    c[len(c)-1].Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_05_nsubjets/avg_nsubjet_'+hist.GetName()+'.pdf')

histList_n = [h3_b0_n,h4_b0_n,h5_b0_n,h3_b1_n,h4_b1_n,h5_b1_n]
c_n=[]
for hist in histList_n:
    hist.GetXaxis().SetTitle('jet p_{T} [GeV]')
    hist.GetYaxis().SetTitle('jet |#eta|')
    hist.SetMarkerSize(2)
    c_n.append(ROOT.TCanvas('c_N_'+str(len(c_n)),'c_N_'+str(len(c_n)),800,600))
    c_n[len(c_n)-1].cd()
    hist.Draw('text45')
    c_n[len(c_n)-1].Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_05_nsubjets/nentries_'+hist.GetName()+'.pdf')
