#!/usr/bin/env python
import argparse,ROOT,os,glob,sys,subprocess
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
ROOT.gROOT.SetBatch()
parser = argparse.ArgumentParser(add_help=False, description='Plot N_subjets')
parser.add_argument('ptBin')
parser.add_argument('nSubjetBin')
parser.add_argument('bTag')
args = parser.parse_args()
fileNameList = ['../kinematic_hists/pythia_subjetTemplbMatchFix70/main_kinematic_pythia_subjetTemplbMatchFix70.root',
            '../kinematic_hists/ttbar_subjetTemplbMatchFix70/main_kinematic_ttbar_subjetTemplbMatchFix70.root',
            '../kinematic_hists/RPV10_subjetTemplbMatchFix70/main_kinematic_RPV10_subjetTemplbMatchFix70.root',
            '../kinematic_hists/ICHEP_subjetTemplbMatchFix70/main_kinematic_ICHEP_subjetTemplbMatchFix70.root']

ptBins = [0.2,0.221,0.244,0.270,0.293,0.329,0.364,0.402,0.445,0.492,0.544,0.6,0.644,0.733,0.811,0.896]

fileSuffix = 'ptBin'+str(args.ptBin)+'_nSubjetBin'+str(args.nSubjetBin)+'_b'+str(args.bTag)
ptStr = str(ptBins[int(args.ptBin)-1])+' TeV < p_{T} < '+str(ptBins[int(args.ptBin)])+' TeV'
colorList = [ROOT.kBlue,ROOT.kGreen,ROOT.kRed,ROOT.kBlack]
legLabels = ['pythia multijet','t#bar{t}','RPV10','data']
can = ROOT.TCanvas('c_'+str(args.ptBin)+'_'+str(args.nSubjetBin)+'_'+str(args.bTag))
can.cd()
histList = []
fileList = [ ROOT.TFile.Open(fileName) for fileName in fileNameList]
drawCount = 0
leg = ROOT.TLegend(0.65,0.65,0.8,0.825)
leg.SetBorderSize(0)
leg.SetFillStyle(0)
leg.SetTextSize(0.04)
for i in range(len(fileList)):
    f=fileList[i]
    histName ='templ_b'+str(args.bTag)+'_subjetBin'+str(args.nSubjetBin)+'_ptBin'+str(args.ptBin)
    histList.append(f.Get(histName))
    h = histList[-1]
    h.Scale(1./h.Integral())
    h.SetLineColor(colorList[i])
    h.SetLineWidth(2)
    h.SetMaximum(0.35)
    h.GetXaxis().SetTitle('log(m/p_{T})')
    h.GetXaxis().SetTitleSize(0.05)
    h.GetXaxis().SetTitleOffset(1.3)
    h.GetYaxis().SetTitle('Fraction of jets')
    h.GetYaxis().SetTitleSize(0.05)
    h.GetYaxis().SetTitleOffset(1.3)
    leg.AddEntry(h,legLabels[i],'F')
    if not h:
        print 'Histogram not found:',histName
        sys.exit(1)
    if drawCount == 0:
        h.Draw('hist')
        drawCount+=1
    else:
        h.Draw('hist same')
leg.Draw()
ROOT.ATLASLabel(0.65,0.87,'Internal')
lat = ROOT.TLatex()
lat.DrawLatexNDC(0.2,0.85,ptStr)
if int(args.nSubjetBin) < 4:
    lat.DrawLatexNDC(0.2,0.75,'n_{subjet} = '+args.nSubjetBin)
elif int(args.nSubjetBin) == 4:
    lat.DrawLatexNDC(0.2,0.75,'n_{subjet} #geq 4')
if int(args.bTag) == 1:
    lat.DrawLatexNDC(0.2,0.65,'b-matched')
elif int(args.bTag) == 0:
    lat.DrawLatexNDC(0.2,0.65,'non-b-matched')
elif int(args.bTag) == 9:
    lat.DrawLatexNDC(0.2,0.65,'b-inclusive')

outFileName = '/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_09_templates/template_'+fileSuffix
can.Print(outFileName+'.pdf')
can.Print(outFileName+'.png')
os.system('chmod a+r /global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/09_09_templates/*')
