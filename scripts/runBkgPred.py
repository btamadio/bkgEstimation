#!/usr/bin/env python
import ROOT,os,array,pprint, math,time,glob,sys
from bkgPredictor import bkgPredictor

date ='10_10'
name ='pythia_BDT_PtRatios'
kinFileName = '../kinematic_hists/'+name+'/main_kinematic_'+name+'.root'
kinFile = ROOT.TFile.Open(kinFileName)
if not kinFile:
    print 'file %s not found' % kinFileName
    sys.exit(1)
dressedFileNames = glob.glob('../dressed_hists/'+name+'/*.root')

p = bkgPredictor(kinFile,dressedFileNames)

p.histDict['n4jVR_b0'].Draw('h')
p.histDict['n4jVR_b0'].SetMarkerSize(0)
p.histDict['n4jVR_b0'].SetFillColor(ROOT.kRed)
p.histDict['n4jVR_b0'].SetFillStyle(3001)
p.histDict['n4jVR_b0'].Draw('e2')
#p.obsDict['n4jVR_b0'].Draw('p same')
