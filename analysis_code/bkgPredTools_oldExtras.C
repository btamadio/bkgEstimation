#include "analysis.h"

//char plot_locations[100];
char plot_locations_base[100];
char* hist_locations = (char*)"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation";
char* hname;

void drawJetMass(int jet, string reg, int njet, int btag, int etaCut){
  SetAtlasStyle();
  double lumi = 8.9;
  double dEta_cut = 1.4;

  TFile *f = new TFile(Form("%s/kinematic_hists/dataD2/main_kinematic_dataD2.root",hist_locations)); 
  char* etaStr = (char*)"";
  if(etaCut == 1) etaStr = (char*)"central_";
  if(etaCut == 2) etaStr = (char*)"forward_";
  hname = Form("M%i_%sm%ij%s_b%i",jet,etaStr,njet,reg.c_str(),btag);
  TH1F *h_mass = (TH1F*)f->Get(hname);

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->SetLogy();
  h_mass->Draw("hist");

  char* vartitle = (char*)"Mass of leading jet [TeV]";
  if(jet > 1) vartitle = Form("Mass of jet %i [TeV]",jet);

  h_mass->GetXaxis()->SetRangeUser(0,0.8);
  h_mass->GetXaxis()->SetTitle(vartitle);
  h_mass->GetXaxis()->SetTitleOffset(1.1);
  h_mass->GetYaxis()->SetTitle("Events");
  h_mass->GetYaxis()->SetTitleOffset(0.9);

  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.04);

  double x_loc = 0.55; double y_loc = 0.40;

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  data",lumi);
  cap.DrawLatex(x_loc+0.15,y_loc+0.3,jet_multi);
  
  sprintf(jet_multi, "N_{largeR jet} #geq %d",njet);
  cap.DrawLatex(x_loc+0.15,y_loc+0.22,jet_multi);

  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(x_loc+0.15,y_loc+0.1,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"b-inclusive");}
  cap.DrawLatex(x_loc+0.15,y_loc+0.15,jet_multi);

  if(etaCut == 1){sprintf(jet_multi,"|#eta| < 1.0");}
  if(etaCut == 2){sprintf(jet_multi,"|#eta| #geq 1.0");}
  if(etaCut) cap.DrawLatex(x_loc+0.15,y_loc+0.05,jet_multi);

  c1->SaveAs(Form("%s/plot_m%i_%s%ij%sb%i.pdf",plot_locations,jet,etaStr,njet,reg.c_str(),btag));
  c1->SaveAs(Form("%s/plot_m%i_%s%ij%sb%i.png",plot_locations,jet,etaStr,njet,reg.c_str(),btag));
}




void drawTemplate(int etaBin, int ptBin)
{
  SetAtlasStyle();
  TFile *f = new TFile(Form("%s/kinematic_hists/pythia_splitTemp/main_kinematic_pythia_splitTemp.root",hist_locations));
  hname = Form("templ_etaBin%i_ptBin%i_VR",etaBin,ptBin);
  TH1F* h_VR = (TH1F*)f->Get(hname);
  hname = Form("templ_etaBin%i_ptBin%i_SR",etaBin,ptBin);
  TH1F* h_SR = (TH1F*)f->Get(hname);

  h_SR->Scale(1/h_SR->Integral());
  h_VR->Scale(1/h_VR->Integral());

  TH1F* ratio = (TH1F*)h_SR->Clone("ratio");
  double sr_num, sr_err, vr_num, vr_err,r,r_err;
  for(int i = 1; i < h_SR->GetNbinsX()+1; i++){
    sr_num = h_SR->GetBinContent(i);
    sr_err = h_SR->GetBinError(i);
    vr_num = h_VR->GetBinContent(i);
    vr_err = h_VR->GetBinError(i);

    if(vr_num !=0 && sr_num !=0){
     r = sr_num/vr_num;
     r_err = r*pow(pow(sr_err/sr_num,2)+pow(vr_err/vr_num,2),0.5);
    }
    else{
      r = 0; r_err = 0;
    }
    ratio->SetBinContent(i,r);
    ratio->SetBinError(i,r_err);
  }

  h_VR->SetLineColor(2);
  h_SR->SetLineColor(4);
  ratio->SetLineColor(1);
  ratio->SetMarkerColor(1);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  //c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  //h_SR->GetXaxis()->SetRangeUser(0,2);
  h_SR->GetYaxis()->SetRangeUser(0,1.1*h_SR->GetMaximum());
  h_SR->GetYaxis()->SetTitle("Events (Normalized)");
  h_SR->GetYaxis()->SetTitleSize(18);
  h_SR->GetYaxis()->SetTitleFont(43);
  h_SR->GetYaxis()->SetTitleOffset(1.2
    );
  h_SR->GetYaxis()->SetLabelSize(0.06);
  //h_SR->GetXaxis()->SetLabelOffset(999);
  h_SR->GetXaxis()->SetLabelSize(0);
  h_SR->Draw("hist");
  h_VR->Draw("histsame");

  c_1->cd(2);
  //ratio->GetXaxis()->SetRangeUser(0,2.0);
  
  ratio->GetYaxis()->SetRangeUser(0,2.0);
  ratio->GetYaxis()->SetTitle("SR / VR");
  ratio->GetXaxis()->SetTitle("log(m/pT)");
  ratio->GetYaxis()->SetTitleSize(18);
  ratio->GetYaxis()->SetTitleFont(43);
  ratio->GetYaxis()->SetTitleOffset(1.);
  ratio->GetYaxis()->SetLabelFont(43);
  ratio->GetYaxis()->SetLabelSize(18);
  ratio->GetYaxis()->SetNdivisions(5);
  ratio->GetXaxis()->SetTitleSize(18);
  ratio->GetXaxis()->SetTitleFont(43);
  ratio->GetXaxis()->SetTitleOffset(4.0);
  ratio->GetXaxis()->SetLabelFont(43);
  ratio->GetXaxis()->SetLabelSize(18);
  ratio->SetTitle("");
  
  ratio->Draw("E0");

  c_1->cd(1);

  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(0.60,0.05,"Internal",0.06,0.10);
  TLegend *leg_1 = new TLegend(x_loc-0.45,y_loc+0.05,x_loc-0.25,y_loc+0.25);

  leg_1->AddEntry(h_SR,"SR, |#Delta#eta| < 0.7","F");
  leg_1->AddEntry(h_VR,"VR, |#Delta#eta| > 0.7","F");
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

  char* binLabel = Form("%3.1f < |#eta| < %3.1f",(etaBin-1)*0.5,etaBin*0.5);
  cap.DrawLatex(x_loc-0.42,y_loc-0.05,binLabel);
  binLabel = Form("%3.1f < log(p_{T}/50GeV) < %3.1f",(ptBin-1)*0.1+1.285,ptBin*0.1+1.285);
  cap.DrawLatex(x_loc-0.48,y_loc-0.15,binLabel);


  char* saveName = Form("%s/plot_template_etaBin%i_ptBin%i.pdf",plot_locations,etaBin,ptBin);
  c_1->SaveAs(saveName);
  saveName = Form("%s/plot_template_etaBin%i_ptBin%i.png",plot_locations,etaBin,ptBin);
  c_1->SaveAs(saveName);
  return;
}


void drawdEta(int btag)
{
  SetAtlasStyle();
  TFile *f = new TFile(Form("%s/kinematic_hists/pythia_splitTemp/main_kinematic_pythia_splitTemp.root",hist_locations));
  hname = Form("dEta_3j_b%i",btag);
  TH1F* h_3j = (TH1F*)f->Get(hname);
  hname = Form("dEta_4j_b%i",btag);
  TH1F* h_4j = (TH1F*)f->Get(hname);
  hname = Form("dEta_5j_b%i",btag);
  TH1F* h_5j = (TH1F*)f->Get(hname);

  h_3j->Rebin(4);
  h_3j->Scale(1/h_3j->Integral());
  h_4j->Rebin(4);
  h_4j->Scale(1/h_4j->Integral());
  h_5j->Rebin(4);
  h_5j->Scale(1/h_5j->Integral());


  TH1F* h_r43 = (TH1F*)h_4j->Clone("h_r43");
  TH1F* h_r53 = (TH1F*)h_5j->Clone("h_r53");
  double num3, num4, num5, err3, err4, err5, r43, r53, r43_err, r53_err;
  for(int i = 1; i < h_3j->GetNbinsX()+1; i++){
    num3= h_3j->GetBinContent(i);
    err3 = h_3j->GetBinError(i);
    num4= h_4j->GetBinContent(i);
    err4 = h_4j->GetBinError(i);
    num5= h_5j->GetBinContent(i);
    err5 = h_5j->GetBinError(i);


    if(num3 !=0 && num4 !=0){
     r43 = num4/num3;
     r43_err = r43*pow(pow(err3/num3,2)+pow(err4/num4,2),0.5);
    }
    else{
      r43 = 0; r43_err = 0;
    }

    if(num3 !=0 && num5 !=0){
     r53 = num5/num3;
     r53_err = r53*pow(pow(err3/num3,2)+pow(err5/num5,2),0.5);
    }
    else{
      r53 = 0; r53_err = 0;
    }
    h_r43->SetBinContent(i,r43);
    h_r43->SetBinError(i,r43_err);
    h_r53->SetBinContent(i,r53);
    h_r53->SetBinError(i,r53_err);
  }

  h_3j->SetLineColor(3);
  h_4j->SetLineColor(4);
  h_5j->SetLineColor(2);

  h_r43->SetLineColor(4);
  h_r43->SetMarkerSize(0.01);
  h_r43->SetMarkerColor(4);
  h_r53->SetLineColor(2);
  h_r53->SetLineColor(2);
  h_r53->SetMarkerSize(0.01);


  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  //c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  //h_SR->GetXaxis()->SetRangeUser(0,2);
  h_3j->GetYaxis()->SetRangeUser(0,1.1*h_5j->GetMaximum());
  h_3j->GetYaxis()->SetTitle("Events (Normalized)");
  h_3j->GetYaxis()->SetTitleSize(18);
  h_3j->GetYaxis()->SetTitleFont(43);
  h_3j->GetYaxis()->SetTitleOffset(1.2
    );
  h_3j->GetYaxis()->SetLabelSize(0.06);
  //h_3j->GetXaxis()->SetLabelOffset(999);
  h_3j->GetXaxis()->SetLabelSize(0);
  h_3j->Draw("hist");
  h_4j->Draw("histsame");
  h_5j->Draw("histsame");

  c_1->cd(2);
  //ratio->GetXaxis()->SetRangeUser(0,2.0);
  
  h_r43->GetYaxis()->SetRangeUser(0,2.0);
  h_r43->GetYaxis()->SetTitle(". / 3j");
  h_r43->GetXaxis()->SetTitle("|#Delta#eta|");
  h_r43->GetYaxis()->SetTitleSize(18);
  h_r43->GetYaxis()->SetTitleFont(43);
  h_r43->GetYaxis()->SetTitleOffset(1.);
  h_r43->GetYaxis()->SetLabelFont(43);
  h_r43->GetYaxis()->SetLabelSize(18);
  h_r43->GetYaxis()->SetNdivisions(5);
  h_r43->GetXaxis()->SetTitleSize(18);
  h_r43->GetXaxis()->SetTitleFont(43);
  h_r43->GetXaxis()->SetTitleOffset(4.0);
  h_r43->GetXaxis()->SetLabelFont(43);
  h_r43->GetXaxis()->SetLabelSize(18);
  h_r43->SetTitle("");
  
  h_r43->Draw("histE0");
  h_r53->Draw("histsameE0");

  c_1->cd(1);

  double x_loc = 0.70; double y_loc = 0.60;
  ATLASLabel(0.12,0.05,"Internal",0.06,0.10);
  TLegend *leg_1 = new TLegend(x_loc-0.05,y_loc+0.05,x_loc+0.15,y_loc+0.25);

  leg_1->AddEntry(h_3j,"n_{jet} = 3","F");
  leg_1->AddEntry(h_4j,"n_{jet} = 4","F");
  leg_1->AddEntry(h_5j,"n_{jet} = 5","F");
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

  char* tagLabel;
  if(btag == 0) tagLabel = (char*)"b-veto";
  if(btag == 1) tagLabel = (char*)"b-tag";
  if(btag == 9) tagLabel = (char*)"b-inclusive";
  cap.DrawLatex(x_loc-0.02,y_loc-0.05,tagLabel);


  char* saveName = Form("%s/plot_dEta_b%i.pdf",plot_locations,btag);
  c_1->SaveAs(saveName);
   saveName = Form("%s/plot_dEta_b%i.png",plot_locations,btag);
  c_1->SaveAs(saveName);
  
  return;
 }
void dataMCComp(string name){
  SetAtlasStyle();
  TFile *fd = new TFile(Form("%s/kinematic_hists/dataD2/main_kinematic_dataD2_0.root",hist_locations)); 
  TFile *fs = new TFile(Form("%s/kinematic_hists/sherpa/main_kinematic_sherpa.root",hist_locations)); 
  TFile *fp = new TFile(Form("%s/kinematic_hists/pythia/main_kinematic_pythia.root",hist_locations)); 

  TH1F *hd = (TH1F*)fd->Get(name.c_str());
  TH1F *hs = (TH1F*)fs->Get(name.c_str());
  TH1F *hp = (TH1F*)fp->Get(name.c_str());

  hd->Scale(1/hd->Integral());
  hs->Scale(1/hs->Integral());
  hp->Scale(1/hp->Integral());

  hd->SetLineColor(1);
  hs->SetLineColor(2);
  hp->SetLineColor(4);

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->SetLogy();
  hd->Draw("hist");
  //hd->GetXaxis()->SetRangeUser(0,0.8);
  hd->GetXaxis()->SetTitle(name.c_str());
  hd->GetXaxis()->SetTitleOffset(1.1);
  hd->GetYaxis()->SetTitle("Frac of Events");
  hd->GetYaxis()->SetTitleOffset(1.1);
  hd->Draw("hist");
  hs->Draw("histsame");
  hp->Draw("histsame");

  TLegend *leg_1 = new TLegend(0.6,0.7,0.8,0.9);

  leg_1->AddEntry(hd,"dataD2","F");
  leg_1->AddEntry(hs,"sherpa","F");
  leg_1->AddEntry(hp,"pythia","F");
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();

  c1->SaveAs(Form("%s/plot_%s_dataMC.pdf",plot_locations,name.c_str()));
  c1->SaveAs(Form("%s/plot_%s_dataMC.png",plot_locations,name.c_str()));
}

void compare_MJs(string reg,int njets, int btag,bool incl,string source){

  //calculation done using ratio method.

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 14;
  //double MJ_bins[N_MJ_bins+1] = {0,0.1,0.2,0.3,0.6,0.8,1,2.0};
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

  bool blindPlot = (blinded && reg == "SR" && njets >= 4);

  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " Prediction for " << source << endl;
  gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(blindPlot){
    for(int i = 61; i < 201; i++){
      h_kin->SetBinContent(i,0);
      h_kin->SetBinError(i,0);}
  }

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //first open all files and calculate ratio
  
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  TH1F* ratio_calc = new TH1F("ratio_calc","ratio_calc",1e3,0,1);
  TH1F* ratio_calc_up = new TH1F("ratio_calc_up","ratio_calc_up",1e3,0,1);
  TH1F* ratio_calc_down = new TH1F("ratio_calc_down","ratio_calc_down",1e3,0,1);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);
    //Normalize!
    temp_MJs[PE]->Scale(obs_NR/temp_MJs[PE]->Integral(21,60));
    temp_MJs_up[PE]->Scale(obs_NR/temp_MJs_up[PE]->Integral(21,60));
    temp_MJs_down[PE]->Scale(obs_NR/temp_MJs_down[PE]->Integral(21,60));

    double ratio = temp_MJs[PE]->Integral(SRbin,200)/ temp_MJs[PE]->Integral(21,60);
    double ratio_up = temp_MJs_up[PE]->Integral(SRbin,200)/ temp_MJs_up[PE]->Integral(21,60);
    double ratio_down = temp_MJs_down[PE]->Integral(SRbin,200)/ temp_MJs_down[PE]->Integral(21,60);

    ratio_calc->Fill( ratio );
    ratio_calc_up->Fill( ratio_up );
    ratio_calc_down->Fill( ratio_down );
  }
  double ratio_med = GetMedian(ratio_calc); 
  double ratio_rms = ratio_calc->GetRMS();
  double ratio_med_up = GetMedian(ratio_calc_up); 
  double ratio_med_down = GetMedian(ratio_calc_down); 
  
  double pred_error, pred_error_up, pred_error_down, pred;
  pred = obs_NR * ratio_med;
  pred_error = pred * pow( pow(obs_NR_error/obs_NR, 2) + pow(ratio_rms/ratio_med, 2), 0.5);
  pred_error_up = ((ratio_med_up - ratio_med) * obs_NR);
  pred_error_down = ((ratio_med - ratio_med_down) * obs_NR);
  double pred_error2 = (fabs(pred_error_up)+fabs(pred_error_down))/2;

  //Switch to variable binning, then get errors bin by bin. 
  
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin_var",MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),MJ_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),MJ_bins);
  }
  //now calculate errors and bands
  TH1F* h_dress_nom = new TH1F("h_dress_nom","h_dress_nom",N_MJ_bins,MJ_bins);
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_MJ_bins,MJ_bins);
  for(int bin = 1; bin <= N_MJ_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    TH1F* temp_bin_up = new TH1F("temp_bin_up","temp_bin_up",1e7,0,1e7);
    TH1F* temp_bin_down = new TH1F("temp_bin_down","temp_bin_down",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
      temp_bin_up->Fill(temp_MJs_up_var[PE]->GetBinContent(bin));
      temp_bin_down->Fill(temp_MJs_down_var[PE]->GetBinContent(bin));
    }

    double rms = temp_bin->GetRMS();
    double med    = GetMedian(temp_bin);
    double med_up = GetMedian(temp_bin_up);
    double med_down = GetMedian(temp_bin_down);
    double syst_uncert_up = fabs( med_up - med);
    double syst_uncert_down = fabs(med_down - med);
    double total_uncert_up = pow( pow(syst_uncert_up, 2) + pow(rms, 2), 0.5);
    double total_uncert_down = pow( pow(syst_uncert_down, 2) + pow(rms, 2), 0.5);

    h_dress_nom->SetBinContent(bin, med);
    h_dress_stat->SetBinContent(bin, med);
    h_dress_syst_up->SetBinContent(bin, med_up);
    h_dress_syst_down->SetBinContent(bin, med_down);

    h_dress_nom->SetBinError(bin, total_uncert_up);
    //h_dress_nom->SetBinError(bin, rms);
    h_dress_stat->SetBinError(bin, rms);
    h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }

  //Scale bin contents by bin width
  for(int bin = 1; bin <= N_MJ_bins; bin ++){
    h_kin_var->SetBinContent(bin, h_kin_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_var->SetBinError(bin, h_kin_var->GetBinError(bin)/MJ_width[bin-1]);

    h_dress_stat->SetBinContent(bin, h_dress_stat->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinContent(bin, h_dress_nom->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinError(bin, h_dress_nom->GetBinError(bin)/MJ_width[bin-1]);
  }

  TH1F *ratio_nom = (TH1F*)h_kin_var->Clone("ratio_nom");
  ratio_nom->Divide(h_dress_nom);
  ratio_nom->GetXaxis()->SetTitle("M_{J}^{#Sigma} [TeV]");

  h_kin_var->SetLineColor(1);
  h_kin_var->SetMarkerColor(1);
  
  h_dress_nom->SetLineColor(2);
  h_dress_nom->SetMarkerColor(2);
  h_dress_nom->SetMarkerSize(0.001);
  h_dress_nom->SetFillColor(kRed);
  h_dress_nom->SetFillStyle(3010);
  h_dress_stat->SetLineColor(2);
  ratio_nom->SetLineColor(0);
  ratio_nom->SetMarkerColor(1);
  ratio_nom->SetFillColor(kRed);
  ratio_nom->SetFillStyle(3010);

  char c_pred[200];
  char c_pred_int[200];
  char c_obs[200];
  sprintf(c_obs,"M_{J,obs}  = %3.1f", obs);
  if(blindPlot) sprintf(c_obs,"data blinded for M_{J} > 0.6");
  sprintf(c_pred,"M_{J,pred} = %3.1f #pm %3.1f #pm %3.1f", pred, pred_error, pred_error2);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_kin_var->GetXaxis()->SetRangeUser(0,2);
  h_kin_var->GetYaxis()->SetRangeUser(max(1.,0.9*h_kin_var->GetMinimum()),3*h_kin_var->GetMaximum());
  h_kin_var->GetYaxis()->SetTitle("Events / Bin width [TeV^{-1}]");
  h_kin_var->GetYaxis()->SetTitleSize(18);
  h_kin_var->GetYaxis()->SetTitleFont(43);
  h_kin_var->GetYaxis()->SetTitleOffset(1.);
  h_kin_var->GetYaxis()->SetLabelSize(0.06);
  h_kin_var->GetXaxis()->SetLabelOffset(999);
  h_kin_var->GetXaxis()->SetLabelSize(0);
  h_kin_var->Draw("E");
  h_dress_nom->Draw("E2same");
  h_dress_stat->Draw("histsame");
  h_kin_var->Draw("Esame");

  c_1->cd(2);
  ratio_nom->GetXaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetTitle("Data/Pred");
  if(isMC) ratio_nom->GetYaxis()->SetTitle("Kin/Pred");
  ratio_nom->GetYaxis()->SetTitleSize(18);
  ratio_nom->GetYaxis()->SetTitleFont(43);
  ratio_nom->GetYaxis()->SetTitleOffset(1.);
  ratio_nom->GetYaxis()->SetLabelFont(43);
  ratio_nom->GetYaxis()->SetLabelSize(18);
  ratio_nom->GetYaxis()->SetNdivisions(5);
  ratio_nom->GetXaxis()->SetTitleSize(18);
  ratio_nom->GetXaxis()->SetTitleFont(43);
  ratio_nom->GetXaxis()->SetTitleOffset(4.0);
  ratio_nom->GetXaxis()->SetLabelFont(43);
  ratio_nom->GetXaxis()->SetLabelSize(18);
  ratio_nom->SetTitle("");
  ratio_nom->Draw("E1");
  ratio_nom->Draw("E2same");
  
  c_1->cd(1);

  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.06,0.10);
  TLegend leg_1(x_loc-0.05,y_loc-0.05,x_loc+0.15,y_loc+0.15);

  char* kin_name = (char*)"Data";
  if(isMC) kin_name = (char*)"Kinematic";

  leg_1.AddEntry(h_kin,kin_name,"LP");
  leg_1.AddEntry(h_dress_nom,"Prediction","LF");
  leg_1.SetLineColor(0);
  leg_1.SetTextSize(0.06);
  leg_1.SetShadowColor(0);
  leg_1.SetFillStyle(0);
  leg_1.SetFillColor(0);
  leg_1.Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);
  if(njets >= 4 && !blindPlot){
  cap.DrawLatex(x_loc-0.05,y_loc+0.18,c_obs);
  cap.DrawLatex(x_loc-0.05,y_loc+0.25,c_pred);}

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  data",lumi);
  if(isMC) sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.15,0.25,jet_multi);

  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.15,0.1,jet_multi);


  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.15,0.17,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.pdf",plot_locations,region,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.png",plot_locations,region,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);

 if(!incl) texfile << Form("%i &",njets);
 if(incl) texfile << Form("$ \\geq %i$ &",njets);
 texfile << jet_multi;
 
if(reg == "CR") texfile <<"& $ > 1.4$ &";
if(reg == "VR") texfile <<Form("& $ > %3.1f$ &",dEta_cut);
if(reg == "SR") texfile <<Form("& $ < %3.1f$ &",dEta_cut);

 if(blindPlot) {
  texfile << Form("\\textbf{\\textit{blinded}} &");
  texfile << Form(" $%3.1f \\pm %3.1f \\pm %3.1f$ \\\\[0.2em] \n", pred, pred_error, pred_error2);
}
 else {
  if(isMC) texfile << Form("$%3.1f \\pm %3.1f$ &", obs, obs_error);
  else texfile << Form("$%i$ &", (int)obs);
  texfile << Form(" $%3.1f \\pm %3.1f \\pm %3.1f$", pred, pred_error, pred_error2);
  texfile << Form("& $%3.3f$ \\\\[0.2em] \n", dev);
}
  return;
}

void compare_MJ_and_Signal(string reg,int njets, int btag,bool incl,string source){
  //Version of compare_MJs() used for signal injection test

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 14;
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << endl;
  gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  h_kin->Scale(lumi);

  sprintf(f_kinematic,"%s/kinematic_hists/pythia/main_kinematic_pythia.root",hist_locations);
  TFile *f_kin_bkg = TFile::Open(f_kinematic);
  TH1F* h_kin_bkg = (TH1F*)f_kin_bkg->Get(nom);
  h_kin_bkg->Scale(lumi);

  sprintf(f_kinematic,"%s/kinematic_hists/ttbar/main_kinematic_ttbar.root",hist_locations);
  TFile *f_kin_sig = TFile::Open(f_kinematic);
  TH1F* h_kin_sig = (TH1F*)f_kin_sig->Get(nom);
  if(source == "pythia_ttbarx3") h_kin_sig->Scale(3);
  h_kin_sig->Scale(lumi);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //Open all files and calculate yield distributions
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  TH1F* yield_SR = new TH1F("yield_SR","yield_SR",1e4,0,1e4);
  TH1F* yield_SR_up = new TH1F("yield_SR_up","yield_SR_up",1e4,0,1e4);
  TH1F* yield_SR_down = new TH1F("yield_SR_down","yield_SR_down",1e4,0,1e4);
  TH1F* yield_NR = new TH1F("yield_NR","yield_NR",1e6,0,1e6);
  TH1F* yield_NR_up = new TH1F("yield_NR_up","yield_NR_up",1e6,0,1e6);
  TH1F* yield_NR_down = new TH1F("yield_NR_down","yield_NR_down",1e6,0,1e6);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

    yield_SR->Fill(temp_MJs[PE]->Integral(SRbin,200));
    yield_SR_up->Fill(temp_MJs_up[PE]->Integral(SRbin,200));
    yield_SR_down->Fill(temp_MJs_down[PE]->Integral(SRbin,200));
    yield_NR->Fill(temp_MJs[PE]->Integral(21,60));
    yield_NR_up->Fill(temp_MJs_up[PE]->Integral(21,60));
    yield_NR_down->Fill(temp_MJs_down[PE]->Integral(21,60));

  }

  double med_SR = GetMedian(yield_SR);
  double med_SR_up = GetMedian(yield_SR_up);
  double med_SR_down = GetMedian(yield_SR_down);
  double rms_SR = yield_SR->GetRMS();
  double med_NR = GetMedian(yield_NR);
  double med_NR_up = GetMedian(yield_NR_up);
  double med_NR_down = GetMedian(yield_NR_down);
  double rms_NR = yield_NR->GetRMS();
  double norm = obs_NR/med_NR;

  double pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  double pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);

  //Switch to variable binning, then get errors bin by bin. 
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin_var",MJ_bins);
  TH1F* h_kin_bkg_var = (TH1F*)h_kin_bkg->Rebin(N_MJ_bins,"h_kin_bkg_var",MJ_bins);
  TH1F* h_kin_sig_var = (TH1F*)h_kin_sig->Rebin(N_MJ_bins,"h_kin_sig_var",MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),MJ_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),MJ_bins);
  }

  //now calculate errors and bands
  TH1F* h_dress_nom = new TH1F("h_dress_nom","h_dress_nom",N_MJ_bins,MJ_bins);
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_MJ_bins,MJ_bins);
  for(int bin = 1; bin <= N_MJ_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    TH1F* temp_bin_up = new TH1F("temp_bin_up","temp_bin_up",1e7,0,1e7);
    TH1F* temp_bin_down = new TH1F("temp_bin_down","temp_bin_down",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
      temp_bin_up->Fill(temp_MJs_up_var[PE]->GetBinContent(bin));
      temp_bin_down->Fill(temp_MJs_down_var[PE]->GetBinContent(bin));
    }

    double rms = temp_bin->GetRMS();
    double med    = GetMedian(temp_bin);
    double med_up = GetMedian(temp_bin_up);
    double med_down = GetMedian(temp_bin_down);
    double syst_uncert_up = fabs( med_up - med);
    double syst_uncert_down = fabs(med_down - med);
    double syst_uncert_avg = 0.5*(syst_uncert_up + syst_uncert_down);
    double total_uncert_up = pow( pow(syst_uncert_up, 2) + pow(rms, 2), 0.5);
    double total_uncert_down = pow( pow(syst_uncert_down, 2) + pow(rms, 2), 0.5);

    double total_uncert = pow(pow(syst_uncert_avg,2)+pow(rms,2),0.5);

    h_dress_nom->SetBinContent(bin, med);
    h_dress_stat->SetBinContent(bin, med);
    h_dress_syst_up->SetBinContent(bin, med_up);
    h_dress_syst_down->SetBinContent(bin, med_down);

    h_dress_nom->SetBinError(bin, total_uncert);
    h_dress_stat->SetBinError(bin, 0);
    h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }
  h_dress_nom->Scale(norm);
  h_dress_stat->Scale(norm);

  //Scale bin contents by bin width
  for(int bin = 1; bin <= N_MJ_bins; bin ++){
    h_kin_var->SetBinContent(bin, h_kin_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_var->SetBinError(bin, h_kin_var->GetBinError(bin)/MJ_width[bin-1]);

    h_kin_bkg_var->SetBinContent(bin, h_kin_bkg_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_bkg_var->SetBinError(bin, h_kin_bkg_var->GetBinError(bin)/MJ_width[bin-1]);

    h_kin_sig_var->SetBinContent(bin, h_kin_sig_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_sig_var->SetBinError(bin, h_kin_sig_var->GetBinError(bin)/MJ_width[bin-1]);

    h_dress_stat->SetBinContent(bin, h_dress_stat->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinContent(bin, h_dress_nom->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinError(bin, h_dress_nom->GetBinError(bin)/MJ_width[bin-1]);
  }


  TH1F *ratio_nom = (TH1F*)h_kin_var->Clone("ratio_nom");
  ratio_nom->Divide(h_dress_nom);
  ratio_nom->GetXaxis()->SetTitle("M_{J}^{#Sigma} [TeV]");

  h_kin_var->SetLineColor(1);
  h_kin_var->SetMarkerColor(1);

  h_kin_bkg_var->SetLineColor(1);
  h_kin_bkg_var->SetLineStyle(4);
  h_kin_sig_var->SetLineColor(8);
  h_kin_sig_var->SetLineStyle(3);
  h_kin_sig_var->SetLineWidth(2);
  
  h_dress_nom->SetLineColor(2);
  h_dress_nom->SetMarkerColor(2);
  h_dress_nom->SetMarkerSize(0.001);
  h_dress_nom->SetFillColor(kRed);
  h_dress_nom->SetFillStyle(3010);
  h_dress_stat->SetLineColor(2);
  ratio_nom->SetLineColor(0);
  ratio_nom->SetMarkerColor(1);
  ratio_nom->SetFillColor(kRed);
  ratio_nom->SetFillStyle(3010);

  char c_pred[200];
  char c_pred_int[200];
  char c_obs[200];
  sprintf(c_obs,"M_{J,obs}  = %3.1f #pm %3.1f", obs, obs_error);
  if(blinded) sprintf(c_obs,"data blinded for M_{J} > 0.6");
  sprintf(c_pred,"M_{J,pred} = %3.1f #pm %3.1f #pm %3.1f", pred, pred_error, pred_error2);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_kin_var->GetXaxis()->SetRangeUser(0,2);
  h_kin_var->GetYaxis()->SetRangeUser(max(1.,0.9*h_kin_var->GetMinimum()),3*h_kin_var->GetMaximum());
  h_kin_var->GetYaxis()->SetTitle("Events / Bin width [TeV^{-1}]");
  h_kin_var->GetYaxis()->SetTitleSize(18);
  h_kin_var->GetYaxis()->SetTitleFont(43);
  h_kin_var->GetYaxis()->SetTitleOffset(1.);
  h_kin_var->GetYaxis()->SetLabelSize(0.06);
  h_kin_var->GetXaxis()->SetLabelOffset(999);
  h_kin_var->GetXaxis()->SetLabelSize(0);
  h_kin_var->Draw("E");
  h_dress_nom->Draw("E2same");
  h_dress_stat->Draw("histsame");
  h_kin_var->Draw("Esame");
  h_kin_bkg_var->Draw("histsame");
  h_kin_sig_var->Draw("histsame");

  c_1->cd(2);
  ratio_nom->GetXaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetTitle("Kin/Pred");
  ratio_nom->GetYaxis()->SetTitleSize(18);
  ratio_nom->GetYaxis()->SetTitleFont(43);
  ratio_nom->GetYaxis()->SetTitleOffset(1.);
  ratio_nom->GetYaxis()->SetLabelFont(43);
  ratio_nom->GetYaxis()->SetLabelSize(18);
  ratio_nom->GetYaxis()->SetNdivisions(5);
  ratio_nom->GetXaxis()->SetTitleSize(18);
  ratio_nom->GetXaxis()->SetTitleFont(43);
  ratio_nom->GetXaxis()->SetTitleOffset(4.0);
  ratio_nom->GetXaxis()->SetLabelFont(43);
  ratio_nom->GetXaxis()->SetLabelSize(18);
  ratio_nom->SetTitle("");
  ratio_nom->Draw("E1");
  ratio_nom->Draw("E2same");
  
  c_1->cd(1);

  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.06,0.10);
  TLegend* leg_1 = new TLegend(x_loc-0.15,y_loc-0.1,x_loc+0.15,y_loc+0.1);

  char* kin_name = (char*)"Kinematic (B+ttbar)";
  if(source == "pythia_ttbarx3") kin_name = (char*)"Kinematic (B+ttbarx3)";


  leg_1->AddEntry(h_kin,kin_name,"LP");
  leg_1->AddEntry(h_dress_nom,"Prediction","LF");
  leg_1->AddEntry(h_kin_bkg_var,"Pythia Bkg","L");
  //leg_1->AddEntry(h_kin_sig_var,"m_{g} = 1200 GeV, m_{#Chi} = 450 GeV","L");
  if(source == "pythia_ttbar") leg_1->AddEntry(h_kin_sig_var,"ttbar","L");
  if(source == "pythia_ttbarx3") leg_1->AddEntry(h_kin_sig_var,"ttbarx3","L");
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);
  cap.DrawLatex(x_loc-0.05,y_loc+0.18,c_obs);
  cap.DrawLatex(x_loc-0.05,y_loc+0.25,c_pred);

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.15,0.25,jet_multi);

  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.15,0.1,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.15,0.17,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.pdf",plot_locations,region,SR_cut_str,region, source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.png",plot_locations,region,SR_cut_str,region, source.c_str());
  c_1->SaveAs(save_name);

 if(!incl) texfile << Form("%i &",njets);
 if(incl) texfile << Form("$ \\geq %i$ &",njets);
 texfile << jet_multi;
 
if(reg == "CR") texfile << "& $ > 1.4$ &";
if(reg == "VR") texfile << Form("& $ > %3.1f$ &",dEta_cut);
if(reg == "SR") texfile << Form("& $ < %3.1f$ &",dEta_cut);

 if(blinded) texfile << Form("\\textbf{\\textit{blinded}} &");
 else texfile << Form("$%3.1f \\pm %3.1f$ &", obs, obs_error);
 texfile << Form(" $%3.1f \\pm %3.1f \\pm %3.1f$ \\\\[0.2em] \n", pred, pred_error, pred_error2);
 //texfile << Form("& $%3.3f$ \\\\[0.2em] \n", dev);
  return;
}


void draw_ratios_th1(double nominal, double mean, double std_dev, TH1F* ratio_hist, char plot_locations[200], char* region, string source){

  TH1F* ratio_draw = new TH1F("ratio_draw","ratio_draw",15,mean-5*std_dev,mean+5*std_dev);
  for(int temp = 1; temp <= ratio_hist->GetNbinsX();temp++){
    ratio_draw->Fill(ratio_hist->GetBinLowEdge(temp)+.00001, ratio_hist->GetBinContent(temp));}
  TLine *mean_line = new TLine(mean,0,mean, ratio_draw->GetMaximum()*1.2);
  TLine *nom_line = new TLine(nominal,0,nominal, ratio_draw->GetMaximum()*1.2);
  mean_line->SetLineColor(2);
  nom_line->SetLineColor(4);
  nom_line->SetLineWidth(6);
  mean_line->SetLineWidth(4);

  TCanvas *c_a = new TCanvas("c_a","c_a");
  ratio_draw->GetYaxis()->SetRangeUser(0, ratio_draw->GetMaximum()*1.2);
  ratio_draw->GetYaxis()->SetTitleSize(20);
  ratio_draw->GetYaxis()->SetTitleFont(43);
  ratio_draw->GetYaxis()->SetTitleOffset(1.);
  ratio_draw->GetYaxis()->SetLabelSize(0.04);
  ratio_draw->GetXaxis()->SetTitleSize(20);
  ratio_draw->GetXaxis()->SetTitleFont(43);
  ratio_draw->GetXaxis()->SetTitleOffset(1.);
  ratio_draw->GetXaxis()->SetLabelSize(0.04);
  ratio_draw->GetXaxis()->SetTitle("Ratios");
  ratio_draw->GetYaxis()->SetTitle("Number of PEs");
  ratio_draw->Draw("E1");
  ratio_draw->Draw("histsame");
  nom_line->Draw();
  mean_line->Draw();
  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-.40,y_loc+0.25,"Internal",0.04,0.10);
  TLegend leg_a(x_loc-0.05,y_loc+0.2,x_loc+0.15,y_loc+0.3);

  leg_a.AddEntry(nom_line,"Nominal/Kinematic Ratio","L");
  leg_a.AddEntry(mean_line,"Median of Dressed Ratios","L");
  leg_a.SetLineColor(0);
  leg_a.SetTextSize(0.03);
  leg_a.SetShadowColor(0);
  leg_a.SetFillStyle(0);
  leg_a.SetFillColor(0);
  leg_a.Draw();

  char save_name[300];
  sprintf(save_name,"%s/%s/plot_ratios_SRcut_op6_%s_%s_NS.pdf",plot_locations,region,region, source.c_str());
  c_a->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_ratios_SRcut_0p6_%s_%s_NS.png",plot_locations,region,region, source.c_str());
  c_a->SaveAs(save_name);
  c_a->Close();

  delete c_a;
  delete nom_line; delete mean_line;
  delete ratio_draw;
  
  return;

}

void bkgDeviationTable(){ 
  vector<string> source_vec = {"data2015_etaCut0p7","data2015_etaCut1p0","data2015_etaCut1p4"};

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_data2015",dateStr);
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  ofstream texfile_bkgDev;
  texfile_bkgDev.open(Form("%s/bkgDeviationsTable.tex",plot_locations));
  texfile_bkgDev << "\\documentclass[11pt]{article} \n";
  texfile_bkgDev << "\\usepackage{amsmath} \n";
  texfile_bkgDev << "\\usepackage{booktabs}\n";
  texfile_bkgDev << "\\usepackage{multirow} \n\n";
  

  texfile_bkgDev << "\\begin{document} \n";
  texfile_bkgDev << "\\centering\n";

  texfile_bkgDev << "\\begin{tabular}{c c c c c c c c c}\n";
  texfile_bkgDev << "\\toprule \n";
  texfile_bkgDev << "\\multirow{2}{*}{$n_{jet}$} & \\multirow{2}{*}{b-tagging} &\n"; 
  texfile_bkgDev << "\\multicolumn{3}{c}{low $|\\Delta\\eta|$} & &\\multicolumn{3}{c}{high $|\\Delta\\eta|$} \\\\ \n";
  texfile_bkgDev << "&& $< 0.7$ &$< 1.0$ &$< 1.4$ & &$\\ge 0.7$ &$\\ge 1.0$ &$\\ge 1.4$ \\\\ \n";
  for(int jets = 3; jets < 6; jets++){
    texfile_bkgDev << "\\midrule \n";
    texfile_bkgDev << "\\multirow{2}{*}{";
    if(jets < 4) texfile_bkgDev << Form("%i}",jets);
    if(jets >= 4) texfile_bkgDev << Form("$ \\geq %i$}",jets);
    for(int k = 0; k < 2; k++){
      if(k==0) texfile_bkgDev << "& b-veto";
      if(k==1) texfile_bkgDev << "& b-tag";

      //low eta
      for(int de = 0; de < 3; de++){
        texfile_bkgDev << Form("& %3.1f",bkgDeviation("SR",jets,k,source_vec[de]));
      }
      texfile_bkgDev << "&";
      //high eta
      for(int de = 0; de < 3; de++){
        texfile_bkgDev << Form("& %3.1f",bkgDeviation("VR",jets,k,source_vec[de]));
      }
      texfile_bkgDev << "\\\\" << endl;
    }
  }
  texfile_bkgDev << "\\bottomrule \n";
  texfile_bkgDev << "\\end{tabular} \n";
  texfile_bkgDev << "\\end{document}";
  texfile_bkgDev.close();

  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
}

void bkgChi2Table(){ 
  vector<string> source_vec = {"data2015_etaCut0p7","data2015_etaCut1p0","data2015_etaCut1p4"};

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_data2015",dateStr);
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  ofstream texfile_bkgDev;
  texfile_bkgDev.open(Form("%s/bkgChi2Table.tex",plot_locations));
  texfile_bkgDev << "\\documentclass[11pt]{article} \n";
  texfile_bkgDev << "\\usepackage{amsmath} \n";
  texfile_bkgDev << "\\usepackage{booktabs}\n";
  texfile_bkgDev << "\\usepackage{multirow} \n\n";
  

  texfile_bkgDev << "\\begin{document} \n";
  texfile_bkgDev << "\\centering\n";

  texfile_bkgDev << "\\begin{tabular}{c c c c c c c c c}\n";
  texfile_bkgDev << "\\toprule \n";
  texfile_bkgDev << "\\multirow{2}{*}{$n_{jet}$} & \\multirow{2}{*}{b-tagging} &\n"; 
  texfile_bkgDev << "\\multicolumn{3}{c}{low $|\\Delta\\eta|$} & &\\multicolumn{3}{c}{high $|\\Delta\\eta|$} \\\\ \n";
  texfile_bkgDev << "&& $< 0.7$ &$< 1.0$ &$< 1.4$ & &$\\ge 0.7$ &$\\ge 1.0$ &$\\ge 1.4$ \\\\ \n";
  for(int jets = 3; jets < 6; jets++){
    texfile_bkgDev << "\\midrule \n";
    texfile_bkgDev << "\\multirow{2}{*}{";
    if(jets < 4) texfile_bkgDev << Form("%i}",jets);
    if(jets >= 4) texfile_bkgDev << Form("$ \\geq %i$}",jets);
    for(int k = 0; k < 2; k++){
      if(k==0) texfile_bkgDev << "& b-veto";
      if(k==1) texfile_bkgDev << "& b-tag";

      //low eta
      for(int de = 0; de < 3; de++){
        texfile_bkgDev << Form("& %3.1f",bkgPredChi2("SR",jets,k,source_vec[de]));
      }
      texfile_bkgDev << "&";
      //high eta
      for(int de = 0; de < 3; de++){
        texfile_bkgDev << Form("& %3.1f",bkgPredChi2("VR",jets,k,source_vec[de]));
      }
      texfile_bkgDev << "\\\\" << endl;
    }
  }
  texfile_bkgDev << "\\bottomrule \n";
  texfile_bkgDev << "\\end{tabular} \n";
  texfile_bkgDev << "\\end{document}";
  texfile_bkgDev.close();

  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
}

void bkgDeviationPullPlotAll(){
  //SetAtlasStyle();
  vector<string> source_vec = {"data2015_etaCut0p7","data2015_etaCut1p0","data2015_etaCut1p4"};

  //sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  //gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  const int nReg = 36;
  TH1F *h_pull = new TH1F("h_pull","",nReg,1,nReg+1);
  double y[nReg];
  double x[nReg];
  for(int i = 0; i < nReg; i++) {
    y[i] = i+1;
    h_pull->SetBinContent(i+1,-3);
  }

  int bin = 1;
  for(int s = 0; s < 3; s++){
    for(int jets = 3; jets < 6; jets++){
      for(int b = 0; b < 2; b++){
        h_pull->GetXaxis()->SetBinLabel(bin,Form("%ijSR_b%i",jets,b));//,source_vec[s].substr(15,3).c_str()));
        x[bin] = bkgDeviation("SR",jets,b,source_vec[s]);
        bin++;
        h_pull->GetXaxis()->SetBinLabel(bin,Form("%ijVR_b%i",jets,b));//,source_vec[s].substr(15,3).c_str()));
        x[bin] = bkgDeviation("VR",jets,b,source_vec[s]);
        bin++;
      }
    }
  }
  TH1F *h_pull2 = new TH1F("h_pull2","",3,1,37);
  h_pull2->GetXaxis()->SetBinLabel(1,"|#Delta#eta_{cut}| = 0.7");
  h_pull2->GetXaxis()->SetBinLabel(2,"|#Delta#eta_{cut}| = 1.0");
  h_pull2->GetXaxis()->SetBinLabel(3,"|#Delta#eta_{cut}| = 1.4");
  for(int i = 1; i < 4; i++) h_pull2->SetBinContent(i,-3);

  TCanvas *c1 = new TCanvas("c1","c1");
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  TPad *pad2 = new TPad("pad2","",0,0,1,1);
  pad2->SetFillColor(0);
  pad2->SetFillStyle(4000);
  pad2->SetFrameFillStyle(0);

   pad1->Draw();
   pad1->cd();

  h_pull->Draw("hbar");
  h_pull->SetStats(0);

  TGraph *gr = new TGraph(nReg,x,y);
  gr->SetMarkerStyle(4);
  gr->SetMarkerSize(1);
  h_pull->GetYaxis()->SetRangeUser(-3,3);
  h_pull2->GetYaxis()->SetRangeUser(-3,3);
  h_pull2->GetYaxis()->SetTitle("#frac{Obs-Exp}{Error(Exp)}");
  h_pull2->GetYaxis()->SetTitleOffset(1);

  TH1F* green = new TH1F("green","green",6,-3,3); 
  green->Fill(-0.5,37);
  green->Fill(0.5,37);
  TH1F* yellow = new TH1F("yellow","yellow",6,-3,3);
  yellow->Fill(-1.5,37);
  yellow->Fill(1.5,37);
  green->SetFillColor(kGreen-4);
  green->SetLineWidth(0);
  yellow->SetFillColor(kYellow-4);
  yellow->SetLineWidth(0);

  green->Draw("Ahistsame");
  yellow->Draw("Ahistsame");
  gr->Draw("P");

  pad2->Draw();
  pad2->cd();
  h_pull2->Draw("hbarY+");
  h_pull2->SetStats(0);
  pad2->SetGrid();

c1->SaveAs(Form("%s/bkgDeviationPullPlot.png",plot_locations));
c1->SaveAs(Form("%s/bkgDeviationPullPlot.pdf",plot_locations));


return;
}

void bkgDeviationPullPlot(string source){
  //SetAtlasStyle();

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  const int nReg = 12;
  TH1F *h_pull = new TH1F("h_pull","",nReg,1,nReg+1);
  double y[nReg];
  double x[nReg];
  for(int i = 0; i < nReg; i++) {
    y[i] = i+1;
    h_pull->SetBinContent(i+1,-3);
  }

  int bin = 1;
  for(int jets = 3; jets < 6; jets++){
    for(int b = 0; b < 2; b++){
      h_pull->GetXaxis()->SetBinLabel(bin,Form("%ijSR_b%i",jets,b));//,source_vec[s].substr(15,3).c_str()));
      x[bin] = bkgDeviation("SR",jets,b,source);
      bin++;
    }
  }
  for(int jets = 3; jets < 6; jets++){
    for(int b = 0; b < 2; b++){
      h_pull->GetXaxis()->SetBinLabel(bin,Form("%ijVR_b%i",jets,b));//,source_vec[s].substr(15,3).c_str()));
      x[bin] = bkgDeviation("VR",jets,b,source);
      bin++;
    }
  }
  TH1F *h_pull2 = new TH1F("h_pull2","",2,1,37);
  h_pull2->GetXaxis()->SetBinLabel(1,"|#Delta#eta| < 1.4");
  h_pull2->GetXaxis()->SetBinLabel(2,"|#Delta#eta| #geq 1.4");
  for(int i = 1; i < 4; i++) h_pull2->SetBinContent(i,-3);

  TCanvas *c1 = new TCanvas("c1","c1");
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  TPad *pad2 = new TPad("pad2","",0,0,1,1);
  pad2->SetFillColor(0);
  pad2->SetFillStyle(4000);
  pad2->SetFrameFillStyle(0);

   pad1->Draw();
   pad1->cd();

  h_pull->Draw("hbar");
  h_pull->SetStats(0);

  TGraph *gr = new TGraph(nReg,x,y);
  gr->SetMarkerStyle(4);
  gr->SetMarkerSize(1);
  h_pull->GetYaxis()->SetRangeUser(-3,3);
  h_pull2->GetYaxis()->SetRangeUser(-3,3);
  h_pull2->GetYaxis()->SetTitle("#frac{Obs-Exp}{Error(Exp)}");
  h_pull2->GetYaxis()->SetTitleOffset(1);

  TH1F* green = new TH1F("green","green",6,-3,3); 
  green->Fill(-0.5,37);
  green->Fill(0.5,37);
  TH1F* yellow = new TH1F("yellow","yellow",6,-3,3);
  yellow->Fill(-1.5,37);
  yellow->Fill(1.5,37);
  green->SetFillColor(kGreen-4);
  green->SetLineWidth(0);
  yellow->SetFillColor(kYellow-4);
  yellow->SetLineWidth(0);

  green->Draw("Ahistsame");
  yellow->Draw("Ahistsame");
  gr->Draw("P");

  pad2->Draw();
  pad2->cd();
  h_pull2->Draw("hbarY+");
  h_pull2->SetStats(0);
  pad2->SetGrid();

c1->SaveAs(Form("%s/NR_DeviationPullPlot.png",plot_locations));
c1->SaveAs(Form("%s/NR_DeviationPullPlot.pdf",plot_locations));


return;
}

double bkgDeviation(string reg,int njets, int btag,bool incl,string source){

  //SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  int SRbin = (int)(100*SR_cut+1);

 string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " background deviation with " << source.c_str() << endl;
  //gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);

  //Truth values
  double obs_NR_error, obs_NR;
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //first open all files and calculate ratio
  TH1F* temp_MJs[PE_number+1];
  TH1F* NR_yield = new TH1F("NR_yield","NR_yield",1e7,0,1e7);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);

    double NR_pred = temp_MJs[PE]->Integral(21,60);

    NR_yield->Fill( NR_pred );
  }
  double pred = GetMedian(NR_yield); 
  double pred_rms = NR_yield->GetRMS();

  double dev = (obs_NR-pred)/pred_rms;
  return dev;
}


double bkgPredChi2(string reg,int njets, int btag,bool incl,string source){

  //SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 7;

  double MJ_bins[N_MJ_bins+1] = {0,0.1,0.2,0.3,0.6,0.8,1,2.0};
  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " background prediction with Chi2 for " << source.c_str() << endl;
  //gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);

  //Truth values
  double obs_NR_error, obs_NR;
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //first open all files
  TH1F* temp_MJs[PE_number+1];
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);
    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
  }
  TH1F* h_dress_nom =(TH1F*)h_kin->Clone("h_dress");
  for(int i = 1; i <=200; i++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    for(int PE = 1; PE<=PE_number; PE++){
      temp_bin->Fill(temp_MJs[PE]->GetBinContent(i));
    }
    h_dress_nom->SetBinContent(i,GetMedian(temp_bin));
    delete temp_bin;
  }

  //normalize
  double SF = obs_NR/h_dress_nom->Integral(21,60);
  cout << "Scaling nominal dressed MJ by: " <<SF << endl;
  h_dress_nom->Scale(SF);

  h_kin->SetLineColor(4);
  h_dress_nom->SetLineColor(2);
  h_dress_nom->Draw("hist");
  h_kin->Draw("histsame");

  //rebin
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin_var",MJ_bins);
  TH1F* h_dress_var = (TH1F*)h_dress_nom->Rebin(N_MJ_bins,"h_dress_var",MJ_bins);
  //NR is now bins 3 and 4
  double chi2_NR = pow((h_dress_var->GetBinContent(3) - h_kin_var->GetBinContent(3)),2)/h_dress_var->GetBinContent(3);
        chi2_NR += pow((h_dress_var->GetBinContent(4) - h_kin_var->GetBinContent(4)),2)/h_dress_var->GetBinContent(4);

  return chi2_NR;
}


void compare_MJs_run1Style(string reg,int njets, int btag,bool incl,string source){

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 13;
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.6,0.8,1.0,2.0};


  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << endl;
  gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  //dummy data from pythia PE
  //bool blinded = (source == "data2015" && reg == "SR");
  //if(blinded) sprintf(f_kinematic,"%s/dressed_hists/pythia/main_dressed_pythia_37.root",hist_locations);

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);
  double ratio_true = obs/obs_NR;

  //first open all files and calculate ratio
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  TH1F* ratio_calc = new TH1F("ratio_calc","ratio_calc",1.E7,0,1);
  TH1F* ratio_calc_up = new TH1F("ratio_calc_up","ratio_calc_up",1.E7,0,1);
  TH1F* ratio_calc_down = new TH1F("ratio_calc_down","ratio_calc_down",1.E7,0,1);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);
    //Normalize!
    temp_MJs[PE]->Scale(obs_NR/temp_MJs[PE]->Integral(21,60));
    temp_MJs_up[PE]->Scale(obs_NR/temp_MJs_up[PE]->Integral(21,60));
    temp_MJs_down[PE]->Scale(obs_NR/temp_MJs_down[PE]->Integral(21,60));


    double ratio = temp_MJs[PE]->Integral(SRbin,200)/ temp_MJs[PE]->Integral(21,60);
    double ratio_up = temp_MJs_up[PE]->Integral(SRbin,200)/ temp_MJs_up[PE]->Integral(21,60);
    double ratio_down = temp_MJs_down[PE]->Integral(SRbin,200)/ temp_MJs_down[PE]->Integral(21,60);

    ratio_calc->Fill( ratio );
    ratio_calc_up->Fill( ratio_up );
    ratio_calc_down->Fill( ratio_down );
  }
  double ratio_med = GetMedian(ratio_calc); 
  double ratio_rms = ratio_calc->GetRMS();
  double ratio_med_up = GetMedian(ratio_calc_up); 
  double ratio_med_down = GetMedian(ratio_calc_down); 
  
  double pred_error, pred_error_up, pred_error_down, pred;
  pred = obs_NR * ratio_med;

  double dev = (pred-obs)/obs;

  pred_error = pred * pow( pow(obs_NR_error/obs_NR, 2) + pow(ratio_rms/ratio_med, 2), 0.5);
  pred_error_up = ((ratio_med_up - ratio_med) * obs_NR);
  pred_error_down = ((ratio_med - ratio_med_down) * obs_NR);

  double pred_error2 = (fabs(pred_error_up)+fabs(pred_error_down))/2;

  //draw TH1 of ratios
  //draw_ratios_th1(ratio_true, ratio_med, ratio_rms, ratio_calc, plot_locations, region, source);

  //Switch to variable binning, then get errors bin by bin. 
  
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin_var",MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),MJ_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),MJ_bins);
  }

   //Scale bin contents by bin width
  for(int bin = 1; bin <= N_MJ_bins; bin ++){
    h_kin_var->SetBinContent(bin, h_kin_var->GetBinContent(bin)/MJ_width[bin-1]);
    for(int PE = 1; PE <=PE_number; PE++){
      temp_MJs_var[PE]->SetBinContent(bin,temp_MJs_var[PE]->GetBinContent(bin)/MJ_width[bin-1]);
      temp_MJs_up_var[PE]->SetBinContent(bin,temp_MJs_up_var[PE]->GetBinContent(bin)/MJ_width[bin-1]);
      temp_MJs_down_var[PE]->SetBinContent(bin,temp_MJs_down_var[PE]->GetBinContent(bin)/MJ_width[bin-1]);
    }
  }

  //now calculate errors and bands
  TH1F* h_dress_nom = new TH1F("h_dress_nom","h_dress_nom",N_MJ_bins,MJ_bins);
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_MJ_bins,MJ_bins);
  
  for(int bin = 1; bin <= N_MJ_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    TH1F* temp_bin_up = new TH1F("temp_bin_up","temp_bin_up",1e7,0,1e7);
    TH1F* temp_bin_down = new TH1F("temp_bin_down","temp_bin_down",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
      temp_bin_up->Fill(temp_MJs_up_var[PE]->GetBinContent(bin));
      temp_bin_down->Fill(temp_MJs_down_var[PE]->GetBinContent(bin));
    }
    double rms = temp_bin->GetRMS();
    double med    = GetMedian(temp_bin);
    double med_up = GetMedian(temp_bin_up);
    double med_down = GetMedian(temp_bin_down);
    double syst_uncert_up = fabs( med_up - med);
    double syst_uncert_down = fabs(med_down - med);
    double total_uncert_up = pow( pow(syst_uncert_up, 2) + pow(rms, 2), 0.5);
    double total_uncert_down = pow( pow(syst_uncert_down, 2) + pow(rms, 2), 0.5);

    h_dress_nom->SetBinContent(bin, med);
    h_dress_stat->SetBinContent(bin, med);
    h_dress_syst_up->SetBinContent(bin, med_up);
    h_dress_syst_down->SetBinContent(bin, med_down);

    h_dress_nom->SetBinError(bin, total_uncert_up);
    h_dress_stat->SetBinError(bin, rms);
    h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }

  TH1F *ratio_nom = (TH1F*)h_kin_var->Clone("ratio_nom");
  ratio_nom->Divide(h_dress_nom);
  ratio_nom->GetXaxis()->SetTitle("M_{J}^{#Sigma} [TeV]");

  TH1F *h_uncert = (TH1F*)h_dress_nom->Clone("uncert");

  h_kin_var->SetLineColor(1);
  h_kin_var->SetMarkerColor(1);
  
  h_dress_nom->SetLineColor(kBlue);
  h_dress_nom->SetMarkerColor(2);
  h_dress_nom->SetMarkerSize(0.001);
  h_dress_nom->SetFillColorAlpha(kBlue,0.35);
  h_dress_nom->SetFillStyle(3005);

  h_uncert->SetLineColor(kGreen-8);
  h_uncert->SetFillColor(kGreen-8);
  h_uncert->SetMarkerSize(0);
  ratio_nom->SetLineColor(0);
  ratio_nom->SetMarkerColor(1);
  ratio_nom->SetFillColor(kGreen-8);
  //ratio_nom->SetFillStyle(3010);

  char c_pred[200];
  char c_pred_int[200];
  char c_obs[200];
  sprintf(c_obs,"M_{J,obs}  = %3.1f #pm %3.1f", obs, obs_error);
  if(blinded) sprintf(c_obs,"data blinded for M_{J} > 0.6");
  sprintf(c_pred,"M_{J,pred} = %3.1f #pm %3.1f #pm %3.1f", pred, pred_error, pred_error2);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_kin_var->GetXaxis()->SetRangeUser(0,2);
  h_kin_var->GetYaxis()->SetRangeUser(max(1.,0.9*h_kin_var->GetMinimum()),3*h_kin_var->GetMaximum());
  h_kin_var->GetYaxis()->SetTitle("Events / Bin width [TeV^{-1}]");
  h_kin_var->GetYaxis()->SetTitleSize(18);
  h_kin_var->GetYaxis()->SetTitleFont(43);
  h_kin_var->GetYaxis()->SetTitleOffset(1.);
  h_kin_var->GetYaxis()->SetLabelSize(0.06);
  h_kin_var->GetXaxis()->SetLabelOffset(999);
  h_kin_var->GetXaxis()->SetLabelSize(0);
  h_kin_var->Draw("E");
  h_uncert->Draw("E2same");
  h_dress_nom->Draw("histsame");
  h_kin_var->Draw("Esame");

  c_1->cd(2);
  ratio_nom->GetXaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetTitle("Kin/Pred");
  ratio_nom->GetYaxis()->SetTitleSize(18);
  ratio_nom->GetYaxis()->SetTitleFont(43);
  ratio_nom->GetYaxis()->SetTitleOffset(1.);
  ratio_nom->GetYaxis()->SetLabelFont(43);
  ratio_nom->GetYaxis()->SetLabelSize(18);
  ratio_nom->GetYaxis()->SetNdivisions(5);
  ratio_nom->GetXaxis()->SetTitleSize(18);
  ratio_nom->GetXaxis()->SetTitleFont(43);
  ratio_nom->GetXaxis()->SetTitleOffset(4.0);
  ratio_nom->GetXaxis()->SetLabelFont(43);
  ratio_nom->GetXaxis()->SetLabelSize(18);
  ratio_nom->SetTitle("");
  ratio_nom->Draw("E1");
  ratio_nom->Draw("E2same");
  
  c_1->cd(1);

  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.06,0.10);
  TLegend leg_1(x_loc-0.05,y_loc-0.05,x_loc+0.15,y_loc+0.15);

  char* kin_name = (char*)"Kinematic";
  //if(blinded) kin_name = Form("Kinematic (blinded for M_{J} > %3.1f TeV)",SR_cut);

  leg_1.AddEntry(h_kin,kin_name,"LP");
  leg_1.AddEntry(h_dress_nom,"Prediction","F");
  leg_1.SetLineColor(0);
  leg_1.SetTextSize(0.06);
  leg_1.SetShadowColor(0);
  leg_1.SetFillStyle(0);
  leg_1.SetFillColor(0);
  leg_1.Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);
  cap.DrawLatex(x_loc-0.05,y_loc+0.18,c_obs);
  cap.DrawLatex(x_loc-0.05,y_loc+0.25,c_pred);

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);

  sprintf(jet_multi, "data2015");//,source.c_str());
  //cap.DrawLatex(x_loc-0.25,y_loc+0.2,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.15,0.25,jet_multi);

  char blinded_label[100];
  if(blinded){
    sprintf(blinded_label,"Kinematic sample blinded for M_{J} > %3.1f TeV",SR_cut);
    //cap.DrawLatex(x_loc-0.22,0.66,blinded_label);

  }
  // /cap.SetTextSize(0.07);
  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.15,0.1,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"b-inclusive");}
  cap.DrawLatex(0.15,0.17,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.pdf",plot_locations,region,SR_cut_str,region, source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.png",plot_locations,region,SR_cut_str,region, source.c_str());
  c_1->SaveAs(save_name);

 if(!incl) texfile << Form("%i &",njets);
 if(incl) texfile << Form("$ \\geq %i$ &",njets);
 texfile << jet_multi;
 
if(reg == "CR") texfile <<"& $ > 1.4$ &";
if(reg == "VR") texfile <<Form("& $ > %3.1f$ &",dEta_cut);
if(reg == "SR") texfile <<Form("& $ < %3.1f$ &",dEta_cut);

 if(blinded) texfile << Form("\\textbf{\\textit{blinded}} &");
 else texfile << Form("$%3.1f \\pm %3.1f$ &", obs, obs_error);
 texfile << Form(" $%3.1f \\pm %3.1f \\pm %3.1f$ \\\\[0.2em] \n", pred, pred_error, pred_error2);
 //texfile << Form("& $%3.3f$ \\\\[0.2em] \n", dev);

  return;

}

void drawTemplatesSplit(int etaBin, int ptBin,int btag,string source){

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char templname[100];

  gROOT->ProcessLine(Form(".! mkdir %s/templates",plot_locations));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());
  TFile *f_kin = TFile::Open(f_kinematic);

  TH2F *h_grid = (TH2F*)f_kin->Get(Form("templGrid_b%i",btag));
  TH1F* h_temp[2];

  sprintf(templname,"templ_SR_b%i_etaBin%i_ptBin%i",btag,etaBin,ptBin);
  h_temp[0] = (TH1F*)f_kin->Get(templname);
  h_temp[0]->Scale(1/h_temp[0]->Integral());
  h_temp[0]->SetLineColor(1);
  h_temp[0]->SetMarkerSize(0.001);

  sprintf(templname,"templ_VR_b%i_etaBin%i_ptBin%i",btag,etaBin,ptBin);
  h_temp[1] = (TH1F*)f_kin->Get(templname);
  h_temp[1]->Scale(1/h_temp[1]->Integral());
  h_temp[1]->SetLineColor(2);
  h_temp[1]->SetMarkerSize(0.001);


  TH1F* h_ratio = (TH1F*)h_temp[1]->Clone(Form("ratio"));
  h_ratio->Divide(h_temp[0]);
 // h_ratio->SetLineColor(1);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_temp[0]->GetXaxis()->SetRangeUser(-7,0);
  h_temp[0]->GetYaxis()->SetRangeUser(1e-3,1.5*h_temp[0]->GetMaximum());
  h_temp[0]->GetYaxis()->SetTitle("Events (Normalized)");
  h_temp[0]->GetYaxis()->SetTitleSize(18);
  h_temp[0]->GetYaxis()->SetTitleFont(43);
  h_temp[0]->GetYaxis()->SetTitleOffset(1.2);
  h_temp[0]->GetYaxis()->SetLabelSize(0.06);
  h_temp[0]->GetXaxis()->SetLabelOffset(999);
  h_temp[0]->GetXaxis()->SetLabelSize(0);
  h_temp[0]->Draw("Ehist");
  h_temp[1]->Draw("Ehistsame");

  c_1->cd(2);
  h_ratio->GetXaxis()->SetRangeUser(-7,0);
  h_ratio->GetYaxis()->SetRangeUser(0,2.0);
  h_ratio->GetXaxis()->SetTitle("log2(m/pt)");
  h_ratio->GetYaxis()->SetTitle("VR / SR");
  h_ratio->GetYaxis()->SetTitleSize(18);
  h_ratio->GetYaxis()->SetTitleFont(43);
  h_ratio->GetYaxis()->SetTitleOffset(1.);
  h_ratio->GetYaxis()->SetLabelFont(43);
  h_ratio->GetYaxis()->SetLabelSize(18);
  h_ratio->GetYaxis()->SetNdivisions(5);
  h_ratio->GetXaxis()->SetTitleSize(18);
  h_ratio->GetXaxis()->SetTitleFont(43);
  h_ratio->GetXaxis()->SetTitleOffset(3.6);
  h_ratio->GetXaxis()->SetLabelFont(43);
  h_ratio->GetXaxis()->SetLabelSize(18);
  h_ratio->SetTitle("");
  h_ratio->Draw("E1");
  
  c_1->cd(1);


  ATLASLabel(0.12,0.1,"Internal",0.06,0.10);
  TLegend *leg_1 = new TLegend(0.15,0.7,0.35,0.9);
  leg_1->AddEntry(h_temp[0],"|#Delta#eta| #leq 1.4","F");
  leg_1->AddEntry(h_temp[1],"|#Delta#eta| > 1.4","F");
  
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

  /*
  sprintf(jet_multi, "%3.2ffb^{-1}  data",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  */

 char jet_multi[100];
 double ptLow = h_grid->GetXaxis()->GetBinLowEdge(ptBin)*1e3;
 double ptHigh = h_grid->GetXaxis()->GetBinLowEdge(ptBin+1)*1e3;

  sprintf(jet_multi, "%i GeV < p_{T} < %i GeV",(int)ptLow,(int)ptHigh);
    if(ptBin ==  h_grid->GetNbinsX()){
      sprintf(jet_multi,"p_{T} > %i GeV",(int)ptLow);
    }
  cap.DrawLatex(0.12,0.6,jet_multi);

  sprintf(jet_multi,"%3.1f < |#eta| < %3.1f",(etaBin-1)*0.5,etaBin*0.5);
  cap.DrawLatex(0.18,0.5,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.22,0.4,jet_multi);


  char save_name[300]; 
  sprintf(save_name,"%s/templates/templ_%s_etaBin%i_ptBin%i_b%i_NS.pdf",plot_locations,source.c_str(),etaBin,ptBin,btag);
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/templates/templ_%s_etaBin%i_ptBin%i_b%i_NS.png",plot_locations,source.c_str(),etaBin,ptBin,btag);
  c_1->SaveAs(save_name);
  

  return;

}

void drawMu1516(){

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char templname[100];

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  string source = "data2015";
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());
  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_temp[2];

  h_temp[0] = (TH1F*)f_kin->Get("h_mu");
  h_temp[0]->SetLineColor(4);

  source = "data2016";
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());
  f_kin = TFile::Open(f_kinematic);

  h_temp[1] = (TH1F*)f_kin->Get("h_mu");
  h_temp[1]->SetLineColor(2);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  //h_temp[0]->GetYaxis()->SetRangeUser(1e-3,1.5*h_temp[0]->GetMaximum());
  TGaxis::SetMaxDigits(4);
  h_temp[0]->GetYaxis()->SetTitle("Events");
  h_temp[0]->GetXaxis()->SetTitle("Avg. Interactions Per Crossing");
  h_temp[0]->GetYaxis()->SetTitleSize(0.04);
  h_temp[0]->GetXaxis()->SetTitleSize(0.04);
  //h_temp[0]->GetYaxis()->SetTitleFont(43);
  h_temp[0]->GetYaxis()->SetTitleOffset(1.1);
  h_temp[0]->GetYaxis()->SetLabelSize(0.04);
  h_temp[0]->GetXaxis()->SetLabelSize(0.04);
  //h_temp[0]->GetXaxis()->SetLabelOffset(999);
  //h_temp[0]->GetXaxis()->SetLabelSize(0);
  h_temp[0]->Draw("hist");
  h_temp[1]->Draw("histsame");

  ATLASLabel(0.65,0.7,"Internal",0.04,0.10);
  TLegend *leg_1 = new TLegend(0.65,0.75,0.85,0.9);
  leg_1->AddEntry(h_temp[0],"2015 Data","F");
  leg_1->AddEntry(h_temp[1],"2016 Data","F");
  
  leg_1->SetLineColor(0);
  //leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

 char jet_multi[100];

  source = "data15_data16";

  char save_name[300]; 
  sprintf(save_name,"%s/plot_mu_%s_NS.pdf",plot_locations,source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/plot_mu_%s_NS.png",plot_locations,source.c_str());
  c_1->SaveAs(save_name);
  

  return;

}

void drawTemplates1516(int etaBin, int ptBin,int btag){

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char templname[100];

  gROOT->ProcessLine(Form(".! mkdir %s/templates",plot_locations));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  string source = "data2015";
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());
  TFile *f_kin = TFile::Open(f_kinematic);

  TH2F *h_grid = (TH2F*)f_kin->Get(Form("templGrid_b%i",btag));
  TH1F* h_temp[2];

  sprintf(templname,"templ_b%i_etaBin%i_ptBin%i",btag,etaBin,ptBin);
  h_temp[0] = (TH1F*)f_kin->Get(templname);
  h_temp[0]->Scale(1/h_temp[0]->Integral());
  h_temp[0]->SetLineColor(4);
  h_temp[0]->SetMarkerSize(0.001);

  source = "data2016";
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());
  f_kin = TFile::Open(f_kinematic);

  sprintf(templname,"templ_b%i_etaBin%i_ptBin%i",btag,etaBin,ptBin);
  h_temp[1] = (TH1F*)f_kin->Get(templname);
  h_temp[1]->Scale(1/h_temp[1]->Integral());
  h_temp[1]->SetLineColor(2);
  h_temp[1]->SetMarkerSize(0.001);


  TH1F* h_ratio = (TH1F*)h_temp[1]->Clone(Form("ratio"));
  h_ratio->Divide(h_temp[0]);
 // h_ratio->SetLineColor(1);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_temp[0]->GetXaxis()->SetRangeUser(-7,0);
  h_temp[0]->GetYaxis()->SetRangeUser(1e-3,1.5*h_temp[0]->GetMaximum());
  h_temp[0]->GetYaxis()->SetTitle("Events (Normalized)");
  h_temp[0]->GetYaxis()->SetTitleSize(18);
  h_temp[0]->GetYaxis()->SetTitleFont(43);
  h_temp[0]->GetYaxis()->SetTitleOffset(1.2);
  h_temp[0]->GetYaxis()->SetLabelSize(0.06);
  h_temp[0]->GetXaxis()->SetLabelOffset(999);
  h_temp[0]->GetXaxis()->SetLabelSize(0);
  h_temp[0]->Draw("Ehist");
  h_temp[1]->Draw("Ehistsame");

  c_1->cd(2);
  h_ratio->GetXaxis()->SetRangeUser(-7,0);
  h_ratio->GetYaxis()->SetRangeUser(0,2.0);
  h_ratio->GetXaxis()->SetTitle("log(m/pt)");
  h_ratio->GetYaxis()->SetTitle("data16 / data15");
  h_ratio->GetYaxis()->SetTitleSize(18);
  h_ratio->GetYaxis()->SetTitleFont(43);
  h_ratio->GetYaxis()->SetTitleOffset(1.);
  h_ratio->GetYaxis()->SetLabelFont(43);
  h_ratio->GetYaxis()->SetLabelSize(18);
  h_ratio->GetYaxis()->SetNdivisions(5);
  h_ratio->GetXaxis()->SetTitleSize(18);
  h_ratio->GetXaxis()->SetTitleFont(43);
  h_ratio->GetXaxis()->SetTitleOffset(3.6);
  h_ratio->GetXaxis()->SetLabelFont(43);
  h_ratio->GetXaxis()->SetLabelSize(18);
  h_ratio->SetTitle("");
  h_ratio->Draw("E1");
  
  c_1->cd(1);


  ATLASLabel(0.12,0.1,"Internal",0.06,0.10);
  TLegend *leg_1 = new TLegend(0.15,0.7,0.35,0.9);
  leg_1->AddEntry(h_temp[0],"data2015","F");
  leg_1->AddEntry(h_temp[1],"data2016","F");
  
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

  /*
  sprintf(jet_multi, "%3.2ffb^{-1}  data",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  */

 char jet_multi[100];
 double ptLow = h_grid->GetXaxis()->GetBinLowEdge(ptBin)*1e3;
 double ptHigh = h_grid->GetXaxis()->GetBinLowEdge(ptBin+1)*1e3;

  sprintf(jet_multi, "%i GeV < p_{T} < %i GeV",(int)ptLow,(int)ptHigh);
    if(ptBin ==  h_grid->GetNbinsX()){
      sprintf(jet_multi,"p_{T} > %i GeV",(int)ptLow);
    }
  cap.DrawLatex(0.12,0.6,jet_multi);

  sprintf(jet_multi,"%3.1f < |#eta| < %3.1f",(etaBin-1)*0.5,etaBin*0.5);
  cap.DrawLatex(0.18,0.5,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"b-inclusive");}
  cap.DrawLatex(0.22,0.4,jet_multi);

  source = "data15_data16";

  char save_name[300]; 
  sprintf(save_name,"%s/templates/templ_%s_etaBin%i_ptBin%i_b%i_NS.pdf",plot_locations,source.c_str(),etaBin,ptBin,btag);
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/templates/templ_%s_etaBin%i_ptBin%i_b%i_NS.png",plot_locations,source.c_str(),etaBin,ptBin,btag);
  c_1->SaveAs(save_name);
  

  return;

}

void bkgTableLine(string reg,int njets, int btag,bool incl,string source){
  //used by bkgTable(), makes prediction in NR.

  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " background prediction" << endl;
  //gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(isMC)h_kin->Scale(lumi);

  //Truth values
  double obs_NR_error, obs_NR;
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //first open all files and calculate ratio
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  TH1F* NR_yield = new TH1F("NR_yield","NR_yield",1e7,0,1e7);
  TH1F* NR_yield_up = new TH1F("NR_yield_up","NR_yield_up",1e7,0,1e7);
  TH1F* NR_yield_down = new TH1F("NR_yiel_down","NR_yield_down",1e7,0,1e7);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

  

    double NR_pred = temp_MJs[PE]->Integral(21,60);
    double NR_pred_up = temp_MJs_up[PE]->Integral(21,60);
    double NR_pred_down = temp_MJs_down[PE]->Integral(21,60);

    NR_yield->Fill( NR_pred );
    NR_yield_up->Fill( NR_pred_up );
    NR_yield_down->Fill( NR_pred_down );
  }
  double pred = GetMedian(NR_yield); 
  double pred_up = GetMedian(NR_yield_up); 
  double pred_down = GetMedian(NR_yield_down);
  double pred_rms = NR_yield->GetRMS();

  double pred_error_up = pred_up-pred;
  double pred_error_down = pred - pred_down;

  double pred_error2=(fabs(pred_error_up)+fabs(pred_error_down))/2;
  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);

  double err_tot = pow(pow(pred_rms,2) + pow(pred_error2,2),0.5);
  double dev = (obs_NR-pred)/err_tot;

  char jet_multi[100];
  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  //if(btag == 9){sprintf(jet_multi,"inclusive");}

    char* dEta_str = Form("& $ < %3.1f$",dEta_cut);
  if(reg == "VR") dEta_str = Form("& $ >%3.1f$",dEta_cut);


  // njet & btag & |dEta| cut & pred
  if(!incl) texfile_bkg << Form("%i & ",njets);
  if(incl) texfile_bkg << Form("$ \\geq %i$ &",njets);
  texfile_bkg << jet_multi;
  texfile_bkg << dEta_str;
  texfile_bkg << Form("& $%i$",(int)obs_NR);
  texfile_bkg << Form("& $%3.1f \\pm %3.1f \\pm %3.1f$",pred,pred_rms,pred_error2); 
  texfile_bkg << Form("& $%3.1f$ \\\\ \n",dev);
}

void summaryTableLine(string reg,int njets, int btag,bool incl,string source){
  double mcNonClosure0p6[4] = {0.071,0.030,0.158,0.054};
  double mcNonClosure0p8[4] = {0.278,0.163,0.184,0.048};
  
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  cout << region << "Bacgkround prediction for summary table" << endl;

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //Open all files and get yield distributions
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  TH1F* yield_SR = new TH1F("yield_SR","yield_SR",1e4,0,1e4);
  TH1F* yield_SR_up = new TH1F("yield_SR_up","yield_SR_up",1e4,0,1e4);
  TH1F* yield_SR_down = new TH1F("yield_SR_down","yield_SR_down",1e4,0,1e4);
  TH1F* yield_NR = new TH1F("yield_NR","yield_NR",1e6,0,1e6);
  TH1F* yield_NR_up = new TH1F("yield_NR_up","yield_NR_up",1e6,0,1e6);
  TH1F* yield_NR_down = new TH1F("yield_NR_down","yield_NR_down",1e6,0,1e6);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

    yield_SR->Fill(temp_MJs[PE]->Integral(SRbin,200));
    yield_SR_up->Fill(temp_MJs_up[PE]->Integral(SRbin,200));
    yield_SR_down->Fill(temp_MJs_down[PE]->Integral(SRbin,200));
    yield_NR->Fill(temp_MJs[PE]->Integral(21,60));
    yield_NR_up->Fill(temp_MJs_up[PE]->Integral(21,60));
    yield_NR_down->Fill(temp_MJs_down[PE]->Integral(21,60));

  }

  double med_SR = GetMedian(yield_SR);
  double med_SR_up = GetMedian(yield_SR_up);
  double med_SR_down = GetMedian(yield_SR_down);
  double rms_SR = yield_SR->GetRMS();
  double med_NR = GetMedian(yield_NR);
  double med_NR_up = GetMedian(yield_NR_up);
  double med_NR_down = GetMedian(yield_NR_down);
  double rms_NR = yield_NR->GetRMS();
  double norm = obs_NR/med_NR;

  double pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  //double pred_error = rms_SR*obs_NR/med_NR;
  double pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;


  double pred_error3;
  if(SR_cut == 0.6){
  if(reg == "VR" && btag == 1) pred_error3 = mcNonClosure0p6[0];
  if(reg == "VR" && btag == 9) pred_error3 = mcNonClosure0p6[1];
  if(reg == "SR" && btag == 1) pred_error3 = mcNonClosure0p6[2];
  if(reg == "SR" && btag == 9) pred_error3 = mcNonClosure0p6[3];
}
  if(SR_cut == 0.8){
  if(reg == "VR" && btag == 1) pred_error3 = mcNonClosure0p8[0];
  if(reg == "VR" && btag == 9) pred_error3 = mcNonClosure0p8[1];
  if(reg == "SR" && btag == 1) pred_error3 = mcNonClosure0p8[2];
  if(reg == "SR" && btag == 9) pred_error3 = mcNonClosure0p8[3];
}
  pred_error3 = pred_error3*pred;

  region = Form("%ij%s",njets,reg.c_str());
  if(btag == 1) region = Form("%ij%sb1",njets,reg.c_str());

  //region & obs & pred
  texfile_summary << region;
  if(blinded && reg == "SR") texfile_summary << "& {\\bf NA} &" << endl;
  else texfile_summary << Form("& %i &",(int)obs); 
  texfile_summary << Form(" %3.1f $\\pm$ %3.1f $\\pm$ %3.1f $\\pm$ %3.1f \\\\ \n",pred,pred_error,pred_error2,pred_error3); 
}

void bkgTable(string source){ 
  //makes tex table of predicted and observed values in NR.

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  texfile_bkg.open(Form("%s/Predictions_%s_NR.tex",plot_locations,source.c_str()));
  texfile_bkg << "\\documentclass[11pt]{article} \n";
  texfile_bkg << "\\usepackage{booktabs} \n";
  texfile_bkg << "\\usepackage{amsmath} \n \n";
  texfile_bkg << "\\begin{document} \n";
  texfile_bkg << "\\centering\n";

  texfile_bkg << "\\begin{tabular}{c c c c c c}\n";
  texfile_bkg << "$n_{jet}$ & b-tagging & $|\\Delta\\eta|$ cut & observed & predicted & deviation\\\\ \n";
  texfile_bkg << "\\toprule \n";

  for(int jets = 3; jets < 5; jets++){
    for(int k = 0; k < 2; k++){ //btags
      bkgTableLine("SR",jets,btag[k],0,source);
      bkgTableLine("VR",jets,btag[k],0,source);
    }
  }

  for(int jets = 4; jets < 6; jets++){
    for(int k = 0; k < 2; k++){ 
      bkgTableLine("SR",jets,btag[k],1,source);
      bkgTableLine("VR",jets,btag[k],1,source);
    }
  }
  texfile_bkg << "\\midrule \n";
  texfile_bkg << "\\\\ \\end{tabular} \n";
  texfile_bkg << "\\end{document}";
  texfile_bkg.close();

  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
}

void summaryTable(string reg, string source){
  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  texfile_summary.open(Form("%s/SummaryTable_%s_%s.tex",plot_locations,reg.c_str(),source.c_str()));
  texfile_summary << "\\documentclass[11pt]{article} \n";
  texfile_summary << "\\usepackage{amsmath} \n";
  texfile_summary << "\\usepackage{graphicx} \n \n";
  texfile_summary << "\\begin{document} \n";

  texfile_summary << "\\begin{table} \n";
  texfile_summary << "\\begin{center} \n";
  texfile_summary << "\\vspace{0.3cm} \n";
  texfile_summary << "\\scalebox{1.0}{ \n";
  texfile_summary << "\\hspace*{-0.1cm} \\begin{tabular}{lclclcl} \n";
  texfile_summary << "\\hline \n";
  texfile_summary << "Region & observed & expected \\\\ \n";
  texfile_summary << "\\hline \n";

  for(int jets = 4; jets < 6; jets++){
    for(int k = 1; k < 3; k++){ //btags
      if(jets == 4) SR_cut = 0.8;
      if(jets == 5) SR_cut = 0.6;
      summaryTableLine(reg,jets,btag[k],1,source);
    }
  }

  texfile_summary << "\\hline \n";
  texfile_summary << "\\end{tabular} \n";
  texfile_summary << "} \n";
  texfile_summary << "\\caption{ \n";
  if(reg == "SR") texfile_summary << "Observed and expected background yields in four signal regions.";
  else texfile_summary << "Observed and expected background yields in four validation regions.";
  texfile_summary << "The three uncertainty components of the background prediction are the ";
  texfile_summary << "template and jet mass randomization statistical uncertainty,";
  texfile_summary << "residual \\pt-dependence uncertainty and the Monte Carlo-based non-closure uncertainty. \n}";
  texfile_summary << "\\label{tab:bkgsummary} \n";
  texfile_summary << "\\end{center} \n";
  texfile_summary << "\\end{table} \n";

  texfile_summary << "\\end{document}";

  texfile_summary.close();
  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
}