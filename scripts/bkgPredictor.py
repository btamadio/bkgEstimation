#!/usr/bin/env python
import ROOT,os,array,pprint, math,time,glob,sys

ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasStyle.C')
ROOT.gROOT.LoadMacro('/global/homes/b/btamadio/atlasstyle/AtlasLabels.C')
ROOT.SetAtlasStyle()

def median(lst):
    quotient,remainder = divmod(len(lst),2)
    if remainder:
        return sorted(lst)[quotient]
    return sum(sorted(lst)[quotient-1:quotient+1])/2
def mean(lst):
    return sum(lst) / float(len(lst))

def std(lst):
    xbar = mean(lst)
    std = 0.0
    for entry in lst:
        std += (entry-xbar)*(entry-xbar)
    std /= float(len(lst)-1)
    return math.sqrt(std)

class bkgPredictor:
    def __init__(self,kinFile,dressedFileNames):
        self.kinFile = kinFile
        self.dressedFileNames = dressedFileNames
        self.MJbins = [0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,0.45,0.5,0.6,0.8,1.0,2.0]
        self.obsDict = {}
        self.profileDict = {}
        self.errorDict = {}
        self.binErrorDict = {}
        self.histDict = {}
        self.predDict = {}
        self.normDict = {}
        self.mjCut = 800.0
        self.normRegion = (0.2,0.6)
        self.regionList = ['n3jVR_b0','n3jVR_b1','n4jVR_b0','n4jVR_b1','m5jVR_b0','m5jVR_b1',
                           'n3jSR_b0','n3jSR_b1','n4jSR_b0','n4jSR_b1','m5jSR_b0','m5jSR_b1']
        for name in self.regionList:
            self.profileDict[name] = []
            self.binErrorDict[name] = []
            for i in range(len(self.MJbins)-1):
                self.profileDict[name].append([])
            self.obsDict[name] = kinFile.Get('MJ_'+name).Rebin(len(self.MJbins)-1,name,array.array('d',self.MJbins))
            self.histDict[name] = ROOT.TH1F('pred_'+name,name,len(self.MJbins)-1,array.array('d',self.MJbins))
        self.loopAndFill()
        self.calcErrors()
    def loopAndFill(self):
        #loop over pseudoexperiment files and fill prediction histograms
        for fileName in self.dressedFileNames:
            tfile = ROOT.TFile.Open(fileName)
            for name in self.regionList:
                h=tfile.Get('MJ_'+name).Rebin(len(self.MJbins)-1,name,array.array('d',self.MJbins))
                for bin in range(1,h.GetNbinsX()+1):
                    self.profileDict[name][bin-1].append(h.GetBinContent(bin))
        for name in self.regionList:
            for bin in range(1,self.histDict[name].GetNbinsX()+1):
                self.histDict[name].SetBinContent(bin,mean(self.profileDict[name][bin-1]))
            self.normDict[name] = (self.obsDict[name].Integral(self.obsDict[name].FindBin(self.normRegion[0]),self.obsDict[name].FindBin(self.normRegion[1]))) / (self.histDict[name].Integral(self.histDict[name].FindBin(self.normRegion[0]),self.histDict[name].FindBin(self.normRegion[1]))) 
            self.histDict[name].Scale(self.normDict[name])  
    def calcErrors(self):
        for name in self.regionList:
            for bin in range(1,self.histDict[name].GetNbinsX()+1):
                self.histDict[name].SetBinError(bin,std(self.profileDict[name][bin-1]))
