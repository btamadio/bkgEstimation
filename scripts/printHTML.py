#!/usr/bin/python
outStr = """<HTML>
<HEAD> </HEAD>
<BODY> 
<CENTER><H2 style="color:red">Template Comparisons </H2> 
<TABLE border=3> 
"""
for i in range(1,16):
    for j in [0,1,9]:
        outStr+="""<TR> 
<TD><CENTER><img src="template_ptBin"""+str(i)+"_nSubjetBin1_b"+str(j)+""".png" height="600" width="800"><BR>(<a href="template_ptBin"""+str(i)+"_nSubjetBin1_b"+str(j)+""".png">link</a>) </CENTER></TD> 
<TD><CENTER><img src="template_ptBin"""+str(i)+"_nSubjetBin2_b"+str(j)+""".png" height="600" width="800"><BR>(<a href="template_ptBin"""+str(i)+"_nSubjetBin2_b"+str(j)+""".png">link</a>) </CENTER></TD> 
</TR>
<TR> 
<TD><CENTER><img src="template_ptBin"""+str(i)+"_nSubjetBin3_b"+str(j)+""".png" height="600" width="800"><BR>(<a href="template_ptBin"""+str(i)+"_nSubjetBin3_b"+str(j)+""".png">link</a>) </CENTER></TD> 
<TD><CENTER><img src="template_ptBin"""+str(i)+"_nSubjetBin4_b"+str(j)+""".png" height="600" width="800"><BR>(<a href="template_ptBin"""+str(i)+"_nSubjetBin4_b"+str(j)+""".png">link</a>) </CENTER></TD> 
</TR>
"""

outStr+="""
</TABLE></CENTER> 
</BODY> 
</HTML>"""
print outStr
