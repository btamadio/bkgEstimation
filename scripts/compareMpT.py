#!/usr/bin/env python
import ROOT,os,array,pprint, math,time,argparse,sys
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()
parser = argparse.ArgumentParser(add_help=False, description='make m/pT histograms')
parser.add_argument('name')
parser.add_argument('jobNum')
args = parser.parse_args()
inFile = ROOT.TFile.Open('../samples/'+args.name+'/main_'+args.name+'.root')
t = inFile.Get('miniTree')
nEntries = t.GetEntries()

nSplit = 1000
k =math.ceil(float(nEntries)/float(nSplit))
eventStart = int(k*int(args.jobNum))
eventEnd = int(min(nEntries,(int(args.jobNum)+1)*k))

print 'nEntries = %i, nSplit = %i, k = %f' % (nEntries,nSplit,k)
print 'eventStart = %i, eventEnd = %i' % (eventStart,eventEnd)
if eventStart >= nEntries:
    print 'No remainining entries. Exiting.'
    sys.exit(1)
print '---- Processing events %i through %i' % (eventStart,eventEnd-1)
#sys.exit(0)


bdtBins = [-1.0,0.0,0.3,1.0]
ptBins = [0.2,0.221,0.244,0.270,0.293,0.329,0.364,0.402,0.445,0.492,0.544,0.6,0.644,0.733,0.811,0.896]
bMatch = [0,1]
nJet = [3,4,5]
nBins=30
xLow=-7
xUp=0
histDict = {}

outFile = ROOT.TFile.Open('output/output_'+args.name+'_'+args.jobNum+'.root','RECREATE')

for ipT in range(len(ptBins)-1):
    for iBDT in range(len(bdtBins)-1):
        for iB in bMatch:
            for iJet in nJet:
                histName = 'h_pT_'+str(ipT)+'_BDT_'+str(iBDT)+'_b_'+str(iB)+'_njet_'+str(iJet)
                histDict[(ipT,iBDT,iB,iJet)] = ROOT.TH1F(histName,histName,nBins,xLow,xUp)

for entry in range(eventStart,eventEnd):
    t.GetEntry(entry)
    if entry % 10000 == 0:
        print '--- entry %i ---' % entry
    ptBin = -1
    bdtBin = -1
    for j in range(len(bdtBins)-1):
        if t.BDTG > bdtBins[j] and t.BDTG < bdtBins[j+1]:
            bdtBin = j
            break
    for i in range(min(4,t.jet_pt.size())):
        if t.jet_pt.at(i) >= ptBins[-1]:
            ptBin = len(ptBins)-2
        else:
            for j in range(len(ptBins)-1):
                if t.jet_pt.at(i) >= ptBins[j] and t.jet_pt.at(i) < ptBins[j+1]:
                    ptBin = j
                    break
        qgKey = -1
        if t.jet_qmatched.at(i) == 1:
            qgKey = 1
        elif t.jet_gmatched.at(i) == 1:
            qgKey = 0
        key = (ptBin,bdtBin,qgKey,min(5,t.njet))
        val = -999999
        if t.jet_m.at(i) > 0:
            val = math.log(t.jet_m.at(i)/t.jet_pt.at(i))
        if key not in histDict:
            print 'Warning: could not identify bin. Run %i, Event number %i, jet number %i' % (t.runNumber,t.eventNumber,i)
            print '    pt = %f, BDTG = %f, qgKey = %i, njet = %i' % (t.jet_pt.at(i),t.BDTG,qgKey,t.njet)
        elif val != -999999:#if t.MJ < 0.8:
            histDict[key].Fill(val,t.weight)#*t.bSF_70)
outFile.Write()
outFile.Close()
