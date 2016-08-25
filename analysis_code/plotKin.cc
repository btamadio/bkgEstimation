int plotKin(string reg,int njets, int btag,bool incl,string source){

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char nom[100];
  const int N_MJ_bins = 14;
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

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(blindPlot){
    for(int i = 61; i < 201; i++){
      h_kin->SetBinContent(i,0);
      h_kin->SetBinError(i,0);}
  }
  
  //TH1F* h_kin = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin",MJ_bins);

  h_kin->SetLineColor(1);
  h_kin->SetMarkerColor(1);
  h_kin->GetYaxis()->SetTitle("Events");// / Bin width [TeV^{-1}]");
  h_kin->GetYaxis()->SetTitleOffset(0.8);
  h_kin->GetXaxis()->SetTitle("M_{J}^{#Sigma} [TeV]");
  
  TCanvas *c_1 = new TCanvas("c_1","c_1");
  h_kin->Draw("Esame");

  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.04,0.10);
  TLegend leg_1(x_loc-0.05,y_loc-0.05,x_loc+0.15,y_loc+0.15);

  char* kin_name = (char*)"Debug Stream";
  if(isMC) kin_name = (char*)"Kinematic";

  leg_1.AddEntry(h_kin,kin_name,"LP");
  leg_1.SetLineColor(0);
  leg_1.SetTextSize(0.04);
  leg_1.SetShadowColor(0);
  leg_1.SetFillStyle(0);
  leg_1.SetFillColor(0);
  leg_1.Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.04);

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  data",lumi);
  if(isMC) sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.65,0.37,jet_multi);

  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.65,0.25,jet_multi);


  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.65,0.31,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.pdf",plot_locations,region,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.png",plot_locations,region,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);
return 0;
}
