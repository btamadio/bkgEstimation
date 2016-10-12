#!/usr/bin/env python
import ROOT,os,array,pprint, math, sys
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
ROOT.gROOT.SetBatch()
inFile = ROOT.TFile.Open('output.root')
date ='10_10'
name ='pythia_BDT_PtRatios'

def plotMpT(ptBin,bdtBin,bMatch):
    bdtBins = [-1.0,0.0,0.3,1.0]
    ptBins = [0.2,0.221,0.244,0.270,0.293,0.329,0.364,0.402,0.445,0.492,0.544,0.6,0.644,0.733,0.811,0.896]

    histName = 'h_pT_'+str(ptBin)+'_BDT_'+str(bdtBin)+'_b_'+str(bMatch)+'_njet_'
    hist3 = inFile.Get(histName+'3')
    hist4 = inFile.Get(histName+'4')
    hist5 = inFile.Get(histName+'5')
    if not hist3 or not hist4 or not hist5:
        print 'Histograms not found. Exiting.'
        sys.exit(1)
    can = ROOT.TCanvas('c1','c1',800,800)
    pad1 = ROOT.TPad('pad1','pad1',0,0.3,1,1.0)
    pad1.SetBottomMargin(0.01)
    pad1.Draw()
    pad1.cd()
    pad1.SetLogy()
    hists = [hist3,hist4,hist5]

    colors = [ROOT.kBlack,ROOT.kBlue,ROOT.kRed]
    legLabels = ['n_{jet} = 3','n_{jet} = 4','n_{jet} #geq 5']
    leg = ROOT.TLegend(0.2,0.625,0.45,0.9)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.04)
    for i in range(len(hists)):
        denom = hists[i].Integral()
        if denom > 0:
            hists[i].Scale(1./denom)
        if i == 0:
            hists[i].Draw('ehist')
            hists[i].SetMinimum(1E-6)
            hists[i].SetMaximum(100)
            hists[i].GetYaxis().SetTitle('fraction of jets')
            hists[i].GetYaxis().SetTitleSize(20)
            hists[i].GetYaxis().SetTitleFont(43)
            hists[i].GetYaxis().SetTitleOffset(1.55)
            hists[i].GetYaxis().SetLabelFont(43)
            hists[i].GetYaxis().SetLabelSize(15)
        else:
            hists[i].Draw('ehist same')
        hists[i].SetLineColor(colors[i])
        hists[i].SetMarkerColor(colors[i])
        hists[i].GetXaxis().SetTitle('log(m/p_{T})')
        leg.AddEntry(hists[i],legLabels[i],'F')
    leg.Draw()
    bLabs = ['non-b-matched','b-matched']
    bdtLab = str(bdtBins[bdtBin])+' < BDT < '+str(bdtBins[bdtBin+1])
    ptLab = 'p_{T} > 0.811 TeV'
    if ptBin < len(ptBins)-2:
        ptLab = str(ptBins[ptBin])+' TeV < p_{T} < '+str(ptBins[ptBin+1])+' TeV'
    lat = ROOT.TLatex()
    lat.DrawLatexNDC(0.4,0.85,ptLab)
    lat.DrawLatexNDC(0.4,0.75,bdtLab)
    lat.DrawLatexNDC(0.4,0.65,bLabs[bMatch])
    can.cd()
    pad2 = ROOT.TPad('pad2','pad2',0,0.05,1,0.3)
    pad2.SetTopMargin(0)
    pad2.SetBottomMargin(0.2)
    pad2.SetGridy()
    pad2.Draw()
    pad2.cd()
    ratioHist4 = hist4.Clone(hist4.GetName()+'_rat')
    ratioHist5 = hist5.Clone(hist5.GetName()+'_rat')
    for ratHist in [ratioHist5,ratioHist4]:
        ratHist.Divide(hist3)
        ratHist.SetMarkerStyle(21)
        ratHist.Draw('ep same')
        ratHist.GetYaxis().SetTitle('. / n_{jet} = 3')
        ratHist.SetMinimum(0.1)
        ratHist.SetMaximum(2.5)
        ratHist.GetYaxis().SetNdivisions(505)
        ratHist.GetYaxis().SetTitleSize(20)
        ratHist.GetYaxis().SetTitleFont(43)
        ratHist.GetYaxis().SetTitleOffset(1.55)
        ratHist.GetYaxis().SetLabelFont(43)
        ratHist.GetYaxis().SetLabelSize(15)
        ratHist.GetXaxis().SetTitleSize(20)
        ratHist.GetXaxis().SetTitleFont(43)
        ratHist.GetXaxis().SetTitleOffset(3.8)
        ratHist.GetXaxis().SetLabelFont(43)
        ratHist.GetXaxis().SetLabelSize(15)


    can.Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/'+date+'_'+name+'/'+histName+'compare.png')

for i in range(15):
    for j in range(3):
        plotMpT(i,j,0)
        plotMpT(i,j,1)
os.system('chmod a+r /global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/'+date+'_'+name+'/*')
