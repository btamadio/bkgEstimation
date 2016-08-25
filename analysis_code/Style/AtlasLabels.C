#include "AtlasLabels.h"

#include "TLatex.h"
#include "TLine.h"
#include "TPave.h"
#include "TPad.h"
#include "TMarker.h"


void ATLASLabel(Double_t x,Double_t y,const char* text, double size, double delx, Color_t color) 
{
  //double size = 0.05;
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.SetTextSize(size);

  l.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.SetTextSize(size);
    p.DrawLatex(x+delx,y,text);
  }
}

void ATLASLabelWIP(Double_t x,Double_t y,const char* text, double size, double delx, double next_line, Color_t color)
{
  //double size = 0.05;                                                                                                                                        
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize);                                                                                                        
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.SetTextSize(size);

  l.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p;
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.SetTextSize(size);
    p.DrawLatex(x+delx,y,text);
    p.DrawLatex(x,y-next_line,"Work in Progress");

  }
}
void ATLASLabelOld(Double_t x,Double_t y,bool Preliminary,Color_t color) 
{
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.DrawLatex(x,y,"ATLAS");
  if (Preliminary) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+0.115,y,"Preliminary");
  }
}



void ATLASVersion(const char* version,Double_t x,Double_t y,Color_t color) 
{

  if (version) {
    char versionString[100];
    sprintf(versionString,"Version %s",version);
    TLatex l;
    l.SetTextAlign(22); 
    l.SetTextSize(0.04); 
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextColor(color);
    l.DrawLatex(x,y,versionString);
  }
}

