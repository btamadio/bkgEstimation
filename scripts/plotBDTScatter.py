#!/usr/bin/env python
import ROOT,os,array
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()

date ='10_10'
name ='pythia_BDT_PtRatios'

inFile = ROOT.TFile.Open('../samples/'+name+'/main_'+name+'.root')
t = inFile.Get('miniTree')


drawStrings = [('BDTG','weight*(njet==3)'),
               ('BDTG','weight*(njet==4)'),
               ('BDTG','weight*(njet>=5)'),
               ('abs(jet_eta[1]-jet_eta[0]):BDTG','weight'),
               ('abs(jet_eta[2]-jet_eta[1]):BDTG','weight'),
               ('abs(jet_eta[2]-jet_eta[0]):BDTG','weight'),
               ('abs(jet_pt[1]-jet_pt[0]):BDTG','weight'),
               ('abs(jet_pt[2]-jet_pt[1]):BDTG','weight'),
               ('abs(jet_pt[2]-jet_pt[0]):BDTG','weight'),
               ('(abs(jet_phi[1]-jet_phi[0])<TMath::Pi())*abs(jet_phi[1]-jet_phi[0])+(abs(jet_phi[1]-jet_phi[0])>TMath::Pi())*(2*TMath::Pi()-abs(jet_phi[1]-jet_phi[0])):BDTG','weight'),
               ('(abs(jet_phi[2]-jet_phi[1])<TMath::Pi())*abs(jet_phi[2]-jet_phi[1])+(abs(jet_phi[2]-jet_phi[1])>TMath::Pi())*(2*TMath::Pi()-abs(jet_phi[2]-jet_phi[1])):BDTG','weight'),
               ('(abs(jet_phi[2]-jet_phi[0])<TMath::Pi())*abs(jet_phi[2]-jet_phi[0])+(abs(jet_phi[2]-jet_phi[0])>TMath::Pi())*(2*TMath::Pi()-abs(jet_phi[2]-jet_phi[0])):BDTG','weight')]


xLab = 'BDTG'
yLabs = ['Fraction of events','Fraction of events','Fraction of events',
         '|#Delta #eta_{12}|','|#Delta #eta_{23}|','|#Delta #eta_{13}|',
         '|#Delta #pt_{12}|','|#Delta #pt_{23}|','|#Delta #pt_{13}|',
         '#Delta #phi_{12}','#Delta #phi_{23}','#Delta #phi_{13}']

fileNames = ['BDT_njet3','BDT_njet4','BDT_njet5',
             'BDT_dEta12','BDT_dEta23','BDT_dEta13',
             'BDT_dpT12','BDT_dpT23','BDT_dpT13',
             'BDT_dPhi12','BDT_dPhi23','BDT_dPhi13']
can = []
#for i in range(len(drawStrings)):
#    can.append(ROOT.TCanvas('c_'+str(i),'canvas',800,600))
#    can[-1].cd()
#    if i<3:
#        t.Draw(drawStrings[i][0],drawStrings[i][1],'e hist')
#    else:
#        t.Draw(drawStrings[i][0],drawStrings[i][1],'colz')
#    can[-1].Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/10_07_pythia_BDT_softJet/'+fileNames[i]+'.png')

h_phi12 = ROOT.TH3F('h_phi12','phi12',100,-1*ROOT.Math.Pi(),ROOT.Math.Pi(),100,-1*ROOT.Math.Pi(),ROOT.TMath.Pi(),100,-1,1)
h_phi23 = ROOT.TH3F('h_phi23','phi23',100,-1*ROOT.Math.Pi(),ROOT.Math.Pi(),100,-1*ROOT.Math.Pi(),ROOT.TMath.Pi(),100,-1,1)
h_phi13 = ROOT.TH3F('h_phi13','phi13',100,-1*ROOT.Math.Pi(),ROOT.Math.Pi(),100,-1*ROOT.Math.Pi(),ROOT.TMath.Pi(),100,-1,1)

h_eta12 = ROOT.TH3F('h_eta12','eta12',100,-2,2,100,-2,2,100,-1,1)
h_eta23 = ROOT.TH3F('h_eta23','eta23',100,-2,2,100,-2,2,100,-1,1)
h_eta13 = ROOT.TH3F('h_eta13','eta13',100,-2,2,100,-2,2,100,-1,1)

h_pt12 = ROOT.TH3F('h_pt12','pt12',100,0.2,2,100,0.2,2,100,-1,1)
h_pt23 = ROOT.TH3F('h_pt23','pt23',100,0.2,2,100,0.2,2,100,-1,1)
h_pt13 = ROOT.TH3F('h_pt13','pt13',100,0.2,2,100,0.2,2,100,-1,1)
         
nEntries = t.GetEntries()
#nEntries=10000
for entry in range(nEntries):
    if entry % 10000 == 0:
        print 'entry %i' % entry
    t.GetEntry(entry)
    h_phi12.Fill(t.jet_phi[0],t.jet_phi[1],t.BDTG,t.weight)
    h_phi23.Fill(t.jet_phi[1],t.jet_phi[2],t.BDTG,t.weight)
    h_phi13.Fill(t.jet_phi[0],t.jet_phi[2],t.BDTG,t.weight)
    h_eta12.Fill(t.jet_eta[0],t.jet_eta[1],t.BDTG,t.weight)
    h_eta23.Fill(t.jet_eta[1],t.jet_eta[2],t.BDTG,t.weight)
    h_eta13.Fill(t.jet_eta[0],t.jet_eta[2],t.BDTG,t.weight)
    h_pt12.Fill(t.jet_pt[0],t.jet_pt[1],t.BDTG,t.weight)
    h_pt23.Fill(t.jet_pt[1],t.jet_pt[2],t.BDTG,t.weight)
    h_pt13.Fill(t.jet_pt[0],t.jet_pt[2],t.BDTG,t.weight)

h_phi12_prof = h_phi12.Project3DProfile()
h_phi23_prof = h_phi23.Project3DProfile()
h_phi13_prof = h_phi13.Project3DProfile()
h_eta12_prof = h_eta12.Project3DProfile()
h_eta23_prof = h_eta23.Project3DProfile()
h_eta13_prof = h_eta13.Project3DProfile()
h_pt12_prof = h_pt12.Project3DProfile()
h_pt23_prof = h_pt23.Project3DProfile()
h_pt13_prof = h_pt13.Project3DProfile()
hists = [h_phi12_prof,h_phi23_prof,h_phi13_prof,
         h_eta12_prof,h_eta23_prof,h_eta13_prof,
         h_pt12_prof,h_pt23_prof,h_pt13_prof]
can = []
xLabs = ['#phi_{1}','#phi_{2}','#phi_{1}',
         '#eta_{1}','#eta_{2}','#eta_{1}',
         'p_{T1} [TeV]','p_{T2} [TeV]','p_{T1} [TeV]']
yLabs = ['#phi_{2}','#phi_{3}','#phi_{3}',
         '#eta_{2}','#eta_{3}','#eta_{3}',
         'p_{T2} [TeV]','p_{T3} [TeV]','pt_{3} [TeV]']

for i in range(len(hists)):
    can.append(ROOT.TCanvas('c_'+str(i),'c_'+str(i),800,600))
    can[-1].cd()
    hists[i].Draw('colz')
    hists[i].SetMinimum(-0.75)
    hists[i].SetMaximum(0.75)
    hists[i].GetXaxis().SetTitle(xLabs[i])
    hists[i].GetYaxis().SetTitle(yLabs[i])
    can[-1].Print('/global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/'+date+'_'+name+'/'+hists[i].GetName()+'.png')
    os.system('chmod a+r /global/project/projectdirs/atlas/www/multijet/RPV/btamadio/bkgEstimation/'+date+'_'+name+'/*')
