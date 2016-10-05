#!/usr/bin/env python
import argparse,ROOT,os,glob,sys,subprocess
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
ROOT.gROOT.SetBatch(True)
parser = argparse.ArgumentParser(add_help=False, description='Plot N_subjets')
parser.add_argument('input')
parser.add_argument('ptBin')
parser.add_argument('etaBin')
args = parser.parse_args()

filePath = args.input.strip('/')
print filePath

inFile = ROOT.TFile.Open(filePath)
miniTree = inFile.Get('miniTree')

nBins = 5

etaBins = [0,0.5,1.0,1.5,2.0]
ptBins = [0.2,0.221,0.244,0.270,0.293,0.329,0.364,0.402,0.445,0.492,0.544,0.6,0.644,0.733,0.811,0.896]

if int(args.etaBin) >= len(etaBins) or int(args.ptBin) >= len(ptBins):
    print 'Invalid pt/|eta| bin. Exiting.'
    sys.exit(1)

etaMin = etaBins[int(args.etaBin)-1]
etaMax = etaBins[int(args.etaBin)]
ptMin = ptBins[int(args.ptBin)-1]
ptMax = ptBins[int(args.ptBin)]

ptStr = str(int(1000*ptMin)) + ' GeV < p_{T} < '+str(int(1000*ptMax))+' GeV'
etaStr = str(etaMin) + ' < |#eta| < '+str(etaMax)
lumiStr = '#int L dt = 12.7 fb^{-1}'

lat = ROOT.TLatex()

c0 = ROOT.TCanvas('c0','c0',800,600)
c0.cd()

h3_0 = ROOT.TH1F('h3_0','h3_0',nBins,0.5,nBins+0.5)
h4_0 = ROOT.TH1F('h4_0','h4_0',nBins,0.5,nBins+0.5)
h5_0 = ROOT.TH1F('h5_0','h5_0',nBins,0.5,nBins+0.5)

histList0 = [h3_0,h4_0,h5_0]
for hist in histList0:
    hist.Sumw2()
    hist.SetMarkerSize(0)

selectionStr = 'jet_pt > '+str(ptMin)+' && jet_pt < '+str(ptMax)
selectionStr += ' && fabs(jet_eta) > '+str(etaMin)+' && fabs(jet_eta) < '+str(etaMax)
miniTree.Draw('jet_NTrimSubjets>>h3_0','njet == 3 && jet_bmatched_Fix70 == 0 && '+selectionStr,'e hist')
miniTree.Draw('jet_NTrimSubjets>>h4_0','njet >= 4 && jet_bmatched_Fix70 == 0 && '+selectionStr,'e hist same')
miniTree.Draw('jet_NTrimSubjets>>h5_0','njet >= 5 && jet_bmatched_Fix70 == 0 && '+selectionStr,'e hist same')
h3_0.GetXaxis().SetTitle('N_{subjets}')
h3_0.GetYaxis().SetTitle('Fraction of jets')
h3_0.Scale(1/h3_0.Integral())
h4_0.Scale(1/h4_0.Integral())
h5_0.Scale(1/h5_0.Integral())
h3_0.SetMaximum(0.8)
h3_0.SetLineColor(ROOT.kBlue)
h4_0.SetLineColor(ROOT.kRed)
h5_0.SetLineColor(ROOT.kGreen)

leg0 = ROOT.TLegend(0.65,0.65,0.8,0.825)
leg0.AddEntry(h3_0,'n_{jet} = 3','F')
leg0.AddEntry(h4_0,'n_{jet} = 4','F')
leg0.AddEntry(h5_0,'n_{jet} #geq 5','F')
leg0.SetBorderSize(0)
leg0.SetFillStyle(0)
leg0.SetTextSize(0.04)
leg0.Draw()
ROOT.ATLASLabel(0.65,0.87,'Internal')
lat.DrawLatexNDC(0.2,0.85,ptStr)
lat.DrawLatexNDC(0.2,0.75,etaStr)
lat.DrawLatexNDC(0.65,0.5,lumiStr)
lat.DrawLatexNDC(0.2,0.65,'non-b-matched')

fileSuffix = 'ptBin_'+str(args.ptBin)+'_etaBin_'+str(args.etaBin)
c0.Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_05_nsubjets/nsubjet_b0_'+fileSuffix+'.pdf')

c1 = ROOT.TCanvas('c1','c1',800,600)
c1.cd()

h3_1 = ROOT.TH1F('h3_1','h3_1',nBins,0.5,nBins+0.5)
h4_1 = ROOT.TH1F('h4_1','h4_1',nBins,0.5,nBins+0.5)
h5_1 = ROOT.TH1F('h5_1','h5_1',nBins,0.5,nBins+0.5)
histList1 = [h3_1,h4_1,h5_1]
for hist in histList1:
    hist.Sumw2()
    hist.SetMarkerSize(0)

miniTree.Draw('jet_NTrimSubjets>>h3_1','njet == 3 && jet_bmatched_Fix70 == 1 && '+selectionStr,'e hist')
miniTree.Draw('jet_NTrimSubjets>>h4_1','njet >= 4 && jet_bmatched_Fix70 == 1 && '+selectionStr,'e hist same')
miniTree.Draw('jet_NTrimSubjets>>h5_1','njet >= 5 && jet_bmatched_Fix70 == 1 && '+selectionStr,'e hist same')
h3_1.GetXaxis().SetTitle('N_{subjets}')
h3_1.GetYaxis().SetTitle('Fraction of jets')
h3_1.Scale(1/h3_1.Integral())
h4_1.Scale(1/h4_1.Integral())
h5_1.Scale(1/h5_1.Integral())
h3_1.SetMaximum(0.8)
h3_1.SetLineColor(ROOT.kBlue)
h4_1.SetLineColor(ROOT.kRed)
h5_1.SetLineColor(ROOT.kGreen)

leg1 = ROOT.TLegend(0.65,0.65,0.8,0.825)
leg1.AddEntry(h3_1,'n_{jet} = 3','F')
leg1.AddEntry(h4_1,'n_{jet} = 4','F')
leg1.AddEntry(h5_1,'n_{jet} #geq 5','F')
leg1.SetBorderSize(0)
leg1.SetFillStyle(0)
leg1.SetTextSize(0.04)
leg1.Draw()
ROOT.ATLASLabel(0.65,0.87,'Internal')
lat.DrawLatexNDC(0.2,0.85,ptStr)
lat.DrawLatexNDC(0.2,0.75,etaStr)
lat.DrawLatexNDC(0.65,0.5,lumiStr)
lat.DrawLatexNDC(0.2,0.65,'b-matched')
c1.Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_05_nsubjets/nsubjet_b1_'+fileSuffix+'.pdf')
