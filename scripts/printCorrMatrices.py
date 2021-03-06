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
    h.SetMinimum(-1.0)
    h.SetMaximum(1.0)
    c[-1].Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_22_FullMJ_FullHT_noNTrimSubjets/corrMatrix_data_'+h.GetName()+'.png')
    os.system('chmod a+r /global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_22_FullMJ_FullHT_noNTrimSubjets/*')
