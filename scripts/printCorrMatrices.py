#!/usr/bin/env python
import argparse,ROOT,os,sys,pprint
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
ROOT.gStyle.SetPaintTextFormat('2.2f')
ROOT.gROOT.SetBatch()
parser = argparse.ArgumentParser(add_help=False, description='Plot Correlation Matrices')
parser.add_argument('input')
args = parser.parse_args()
f = ROOT.TFile.Open(args.input)
c = []
i=0
for key in f.GetListOfKeys():
    h = f.Get(key.GetName())
    c.append(ROOT.TCanvas('c_'+str(i),'c_'+str(i),800,600))
    c[-1].cd()
    i+=1
    h.Draw('col0 text')
    h.GetXaxis().SetTickLength(0)
    h.GetYaxis().SetTickLength(0)
    if h.GetNbinsX()==17:
        h.GetXaxis().SetBinLabel(9,'#eta_{1}')
        h.GetXaxis().SetBinLabel(10,'#eta_{2}')
        h.GetXaxis().SetBinLabel(11,'#eta_{3}')
        h.GetYaxis().SetBinLabel(9,'#eta_{1}')
        h.GetYaxis().SetBinLabel(10,'#eta_{2}')
        h.GetYaxis().SetBinLabel(11,'#eta_{3}')
    elif h.GetNbinsX()==22:
        h.GetYaxis().SetBinLabel(11,'#eta_{1}')
        h.GetYaxis().SetBinLabel(12,'#eta_{2}')
        h.GetYaxis().SetBinLabel(13,'#eta_{3}')
        h.GetYaxis().SetBinLabel(14,'#eta_{4}')
    c[-1].Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_20_correlationMatrices_randIndex_3jet/'+h.GetName()+'.png')

