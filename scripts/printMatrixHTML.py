#!/usr/bin/python

def getLabels(njet,btag,dEta):
    labels = ['','','','']
    if njet == 3:
        for i in range(len(labels)):
            labels[i] = 'njet = 3'
    elif njet==4:
        for i in range(len(labels)):
            labels[i] = 'njet = 4'
    elif njet==5:
        for i in range(len(labels)):
            labels[i] = 'njet >= 5'
    if btag == 0:
        for i in range(len(labels)):
            labels[i] += ', b-veto'
    elif btag==1:
        for i in range(len(labels)):
            labels[i] += ', b-tag'
    if dEta == 0:
        for i in range(len(labels)):
            labels[i] += ', |dEta| < 1.4'
    elif dEta == 1:
        for i in range(len(labels)):
            labels[i] += ', |dEta| > 1.4'
    labels[0] += ', MJ < 0.2 TeV'
    labels[1] += ', 0.2 TeV < MJ < 0.6 TeV'
    labels[2] += ', MJ > 0.6 TeV'
    return labels
outStr = """<HTML>
<HEAD> </HEAD>
<BODY> 
<CENTER><H2 style="color:red">Template Comparisons </H2> 
<TABLE border=3> 
"""

mjStr = ['00_02','02_06','06_130','00_130']
dEtaStr = ['00_14','14_40','00_40']
for njet in [3,4,5]:
    for btag in [0,1,9]:
        for dEta in [0,1,2]:
            labels = getLabels(njet,btag,dEta)
            keyStr0='h_n'+str(njet)+'_b'+str(btag)+'_MJ_'+mjStr[0]+'_dEta_'+dEtaStr[dEta]+'.png'
            keyStr1='h_n'+str(njet)+'_b'+str(btag)+'_MJ_'+mjStr[1]+'_dEta_'+dEtaStr[dEta]+'.png'
            keyStr2='h_n'+str(njet)+'_b'+str(btag)+'_MJ_'+mjStr[2]+'_dEta_'+dEtaStr[dEta]+'.png'
            keyStr3='h_n'+str(njet)+'_b'+str(btag)+'_MJ_'+mjStr[3]+'_dEta_'+dEtaStr[dEta]+'.png'
            outStr+='<TR><TD><CENTER><img src="'+keyStr0+'" height="600" width="800"><BR>(<a href="'+keyStr0+'">'+labels[0]+'</a>) </CENTER></TD> <TD><CENTER><img src="'+keyStr1+'" height="600" width="800"><BR>(<a href="'+keyStr1+'">'+labels[1]+'</a>) </CENTER></TD></TR><TR><TD><CENTER><img src="'+keyStr2+'" height="600" width="800"><BR>(<a href="'+keyStr2+'">'+labels[2]+'</a>) </CENTER></TD><TD><CENTER><img src="'+keyStr3+'" height="600" width="800"><BR>(<a href="'+keyStr3+'">'+labels[3]+'</a>) </CENTER></TD></TR>'

outStr+="""
</TABLE></CENTER> 
</BODY> 
</HTML>"""
print outStr
