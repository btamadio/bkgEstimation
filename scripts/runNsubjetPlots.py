#!/usr/bin/env python
import os
etaBins = [0,0.5,1.0,1.5,2.0]
ptBins = [0.2,0.221,0.244,0.270,0.293,0.329,0.364,0.402,0.445,0.492,0.544,0.6,0.644,0.733,0.811,0.896]
for i in range(1,len(ptBins)):
    for j in range(1,len(etaBins)):
        cmd = './plotNsubjetsBin.py ../samples/ICHEP44/main_ICHEP44.root '+str(i)+' '+str(j)
        os.system(cmd)
        
