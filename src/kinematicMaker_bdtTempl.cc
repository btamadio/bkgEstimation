#include "kinematicMaker_bdtTempl.hh"
int main (int argc, char **argv){

  if (argc < 2)
    {
      printf("\nUsage: %s *.root\n\n",argv[0]);
      exit(0);
    }
  
  int const ind = argc - 2;
  TString *path = new TString[ind];
  char* configFile = (char*)argv[1];
  TChain *chain = new TChain("miniTree");

       for ( int j = 0; j < argc-2; j++)
       {
         path[j] = (argv[j+2]); 
         chain->Add(path[j]);
       }

  p = new miniTree(chain);
  Config config;

  if(!isFile(configFile)){
    cout << "config file " << configFile << " not found." << endl;
    exit(1);
  }

  loadConfig(config,configFile);
  
  TFile *f = new TFile("RPV_kin.root", "recreate");
  f->cd();
  nameHistos();
  
  const int nBDTBins = (const int)(config.bdtBins.size()-1);
  //  cout<<"nBDTBins = "<<nBDTBins<<endl;
  double *bdtBins = &(config.bdtBins[0]);
  const int nPtBins = (const int)(config.ptBins.size()-1);
  double *ptBins = &(config.ptBins[0]);

  double logPtLow = log(ptBins[0]/0.05);
  double logPtHigh = log(ptBins[nPtBins]/0.05);

  TH1F* h_mpt[3][nBDTBins][nPtBins];
  TH2F* h_templGrid[3];
  TH2F* h_templGrid_log[3];

  for(int k = 0; k < 3; k++){
    hname = Form("templGrid_b%i",b_tag[k]);
    h_templGrid[k] = new TH2F(hname,hname,nPtBins,ptBins,nBDTBins,bdtBins);
    hname = Form("templGrid_log_b%i",b_tag[k]);
    h_templGrid_log[k] = new TH2F(hname,hname,nPtBins,logPtLow,logPtHigh,nBDTBins,bdtBins[0],bdtBins[nBDTBins]);
    for(int i = 0; i <nBDTBins; i++){
      for(int j = 0; j < nPtBins; j++){
        hname = Form("templ_b%i_bdtBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt[k][i][j] = new TH1F(hname,hname,50,-7,0);
        h_mpt[k][i][j]->Sumw2();
      }
    }
  }

  TH1F* h_dEta = new TH1F("dEta","dEta",20,0,4);

  //initialize loop
  int numev = p->fChain->GetEntries();
  cout << "Total number of events: " << numev << endl;

  //event loop
  for( int nEvent = 0 ; nEvent < numev ; nEvent ++ ){  
    get_event(nEvent);
    if( nEvent%10000 == 0 ) cout << nEvent << " events processed " << endl;
    double wt = p->weight; 
    int njet = p->njet; 
    int btags = -1;
    //    cout<<"wt = "<<wt<<endl;
    if(config.CRdef == 4){ 
      btags = p->nbjet_Fix60; 
      wt*=p->bSF_60;
    }
    if(config.CRdef == 5){ 
      btags = p->nbjet_Fix70; 
      wt*=p->bSF_70;
    }
    if(config.CRdef == 6){ 
      btags = p->nbjet_Fix77; 
      wt*=p->bSF_77;
    }
    if(config.CRdef == 7){ 
      btags = p->nbjet_Fix85; 
      wt*=p->bSF_85;
    }
    if(config.CRdef == 8){ btags = p->nbjet_Flt60; }
    if(config.CRdef == 9){ btags = p->nbjet_Flt70; }
    if(config.CRdef == 10){ btags = p->nbjet_Flt77; }
    if(config.CRdef == 11){ btags = p->nbjet_Flt85; }
    //    cout<<"btags = "<<btags<<endl;

    int b_tag = (int)((bool)btags);
    double dEta_cut = 1.4;

    bool isVR = false;
    if(config.useVR) isVR = (p->dEta > dEta_cut);
    isVR = (int)isVR;
    bool isCR;
    if(config.CRdef == 1) isCR = (njet == 3 && ((isVR && b_tag == 1) || b_tag==0)); //baseline
    if(config.CRdef == 1 && !config.useVR) isCR = (njet == 3);

    if(config.CRdef == 2) isCR = (njet == 3 && ((isVR && b_tag == 1) || b_tag==0) && p->njet_soft ==2 ); //3+soft jets templates
    if(config.CRdef == 3) isCR = (njet == 4 && isVR); //4jVRtempl
    if(config.CRdef >= 4) isCR = (njet == 3 && ((isVR && b_tag == 1) || b_tag==0)); //b-matched templ

    for(int n = 0; n < min(njet,4); n++){
      h_pt[b_tag][n]->Fill((*p->jet_pt)[n],wt);
    }
    h_dEta->Fill(p->dEta,wt);
    //    cout<<"Filled dEta histogram"<<endl;
    double dummyMJ = 0,dummyMass;
    dummyMJ = min(p->MJ,1.99);
    //Fill MJ distributions
    if(njet < 5){
      MJ[njet-3][isVR][2]->Fill(dummyMJ,wt); //b-inclusive
      MJ[njet-3][isVR][b_tag]->Fill(dummyMJ,wt); 
    }
    for(int jet = 4; jet <6; jet++){
      if(njet >= jet){
	MJ[jet-2][isVR][2]->Fill(dummyMJ,wt); 
	MJ[jet-2][isVR][b_tag]->Fill(dummyMJ,wt); 
      }
    }
    //    cout<<"Filled MJ distributions"<<endl;
    ///Fill Mass distributions
    for(int k = 0; k <min(njet,5); k++){
      dummyMass = min((*p->jet_m)[k],1.99);
      bool cent = (fabs((*p->jet_eta)[k]) < 1.0);
        if(njet < 5){
          mass[k][njet-3][isVR][2]->Fill(dummyMass,wt);
          mass[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
          if(cent) {
            mass_central[k][njet-3][isVR][2]->Fill(dummyMass,wt);
            mass_central[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
          }
          else{
            mass_forward[k][njet-3][isVR][2]->Fill(dummyMass,wt);
            mass_forward[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
          }
        }
        for(int jet = 4; jet < 6; jet++){
          if(njet >= jet){
            mass[k][jet-2][isVR][2]->Fill(dummyMass,wt);
            mass[k][jet-2][isVR][b_tag]->Fill(dummyMass,wt);
          if(cent) {
            mass_central[k][jet-2][isVR][2]->Fill(dummyMass,wt);
            mass_central[k][jet-2][isVR][b_tag]->Fill(dummyMass,wt);
          }
          else{
            mass_forward[k][jet-2][isVR][2]->Fill(dummyMass,wt);
            mass_forward[k][jet-2][isVR][b_tag]->Fill(dummyMass,wt);
          }
          }
        }
    }
    //    cout<<"Filled mass distributions"<<endl;

    //Fill templates
    double pt,mass,dummyPt;
    int bin,i,j,l;
    if(isCR){
    for(int k = 0; k < min(njet,4); k++){
        if(config.CRdef == 4) b_tag = (*p->jet_bmatched_Fix60)[k];
	if(config.CRdef == 5) b_tag = (*p->jet_bmatched_Fix70)[k];
        if(config.CRdef == 6) b_tag = (*p->jet_bmatched_Fix77)[k];
        if(config.CRdef == 7) b_tag = (*p->jet_bmatched_Fix85)[k];
        if(config.CRdef == 8) b_tag = (*p->jet_bmatched_Flt60)[k];
        if(config.CRdef == 9) b_tag = (*p->jet_bmatched_Flt70)[k];
        if(config.CRdef == 10) b_tag = (*p->jet_bmatched_Flt77)[k];
        if(config.CRdef == 11) b_tag = (*p->jet_bmatched_Flt85)[k];
	//	cout<<"b_tag = "<<b_tag<<endl;
        pt = (*p->jet_pt)[k];
	//	cout<<"pt = "<<pt<<endl;
        dummyPt = min(pt,config.ptBins[nPtBins]-0.001);
        mass = (*p->jet_m)[k];
	//	cout<<"mass = "<<mass<<endl;
        float BDT_score = p->BDTG;
	//	cout<<"BDT = "<<BDT_score<<endl;
	//	cout<<"dummyPt = "<<dummyPt<<endl;
        h_templGrid_log[b_tag]->Fill(log(dummyPt/0.05),BDT_score,wt);
        bin = h_templGrid[b_tag]->Fill(dummyPt,BDT_score,wt);
	//	cout<<"bin = "<<bin<<endl;
        h_templGrid[b_tag]->GetBinXYZ(bin,j,i,l);
	//	cout<<"i = "<<i<<"\t"<<"j = "<<j<<"\t"<<"l = "<<l<<endl;
        h_mpt[b_tag][i-1][j-1]->Fill(log(mass/pt),wt);
        h_mpt[2][i-1][j-1]->Fill(log(mass/pt),wt);
      }
    }
    //    cout<<"Filled templates"<<endl;


}

f->Write();
f->Close();
return 0;

}

