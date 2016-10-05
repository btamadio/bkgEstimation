#!/usr/bin/env python
import os
for i in range(1,16):
    for j in range(1,5):
        for k in [0,1,9]:
            cmd = './plotTemplates.py '+str(i)+' '+str(j)+' '+str(k)
            os.system(cmd)
