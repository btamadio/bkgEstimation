#!/usr/bin/python
import sys,glob,os
dir = sys.argv[1]
fileList = glob.glob(dir+'/*.root')
for fileName in fileList:
    cmd = 'mv '+fileName+' '+fileName.replace('MJcut','subjetTemplbMatchFix70')
    os.system(cmd)
