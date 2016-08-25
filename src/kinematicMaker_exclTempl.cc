#include "kinematicMaker_exclTempl.hh"
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
  
  const int nEtaBins = (const int)(config.etaBins.size()-1);
  double *etaBins = &(config.etaBins[0]);
  
  const int nPtBins12 = (const int)(config.ptBins12.size()-1);
  double *ptBins12 = &(config.ptBins12[0]);
  const int nPtBins3 = (const int)(config.ptBins3.size()-1);
  double *ptBins3 = &(config.ptBins3[0]);

  TH1F* h_mpt_12[3][nEtaBins][nPtBins12];
  TH1F* h_mpt_3[3][nEtaBins][nPtBins3];
  TH2F* h_templGrid_split[2][3];

  for(int k = 0; k < 3; k++){
    hname = Form("templGrid_12_b%i",b_tag[k]);
    h_templGrid_split[0][k] = new TH2F(hname,hname,nPtBins12,ptBins12,nEtaBins,etaBins);
    hname = Form("templGrid_3_b%i",b_tag[k]);
    h_templGrid_split[1][k] = new TH2F(hname,hname,nPtBins3,ptBins3,nEtaBins,etaBins);
    for(int i = 0; i <nEtaBins; i++){
      for(int j = 0; j < nPtBins12; j++){
        hname = Form("templ_12_b%i_etaBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt_12[k][i][j] = new TH1F(hname,hname,50,-7,0);
        h_mpt_12[k][i][j]->Sumw2();
      }
      for(int j = 0; j < nPtBins3; j++){
        hname = Form("templ_3_b%i_etaBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt_3[k][i][j] = new TH1F(hname,hname,50,-7,0);
        h_mpt_3[k][i][j]->Sumw2();
      }
    }


  } 

  //initialize loop
  int numev = p->fChain->GetEntries();
  cout << " Total number of events " << numev << endl;

  //event loop
  for( int nEvent = 0 ; nEvent < numev ; nEvent ++ ){  
    get_event(nEvent);
    if( nEvent%10000 == 0 ) cout << nEvent << " events processed " << endl;

    double wt = p->weight; 
    int njet = p->njet; 
    int btags = p->nbjet_Fix70;
    //int btags = p->nbjet_Flt70;
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
    if(config.CRdef == 4) isCR = (njet == 3 && ((isVR && b_tag == 1) || b_tag==0)); //b-matched templ

    for(int n = 0; n < min(njet,4); n++){
      h_pt[b_tag][n]->Fill((*p->jet_pt)[n],wt);
    }

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

    //Fill templates
    int bin,i,j,l;
    double pt,eta,mass,dummyPt,dummyEta;
    if(isCR){
      for(int k = 0; k < 2; k++){
        //two leading jets
        if(config.CRdef == 4) b_tag = (*p->jet_bmatched_Fix70)[k];

        pt = (*p->jet_pt)[k];
        dummyPt = min(pt,config.ptBins12[nPtBins12]-0.001);
        eta = (*p->jet_eta)[k];
        dummyEta = min(fabs(eta),config.etaBins[nEtaBins]-0.01);
        mass = (*p->jet_m)[k];

        bin = h_templGrid_split[0][b_tag]->Fill(dummyPt,dummyEta,wt); 
        h_templGrid_split[0][b_tag]->GetBinXYZ(bin,j,i,l);
        h_mpt_12[b_tag][i-1][j-1]->Fill(log(mass/pt),wt);
        h_mpt_12[2][i-1][j-1]->Fill(log(mass/pt),wt);
      }
      for(int k = 2; k < njet; k++){
        //third, maybe fourth jet. 
        if(config.CRdef == 4) b_tag = (*p->jet_bmatched_Fix70)[k];

        pt = (*p->jet_pt)[k];
        dummyPt = min(pt,config.ptBins12[nPtBins3]-0.001);
        eta = (*p->jet_eta)[k];
        dummyEta = min(fabs(eta),config.etaBins[nEtaBins]-0.01);
        mass = (*p->jet_m)[k];

        bin = h_templGrid_split[1][b_tag]->Fill(dummyPt,dummyEta,wt); 
        h_templGrid_split[1][b_tag]->GetBinXYZ(bin,j,i,l);
        h_mpt_3[b_tag][i-1][j-1]->Fill(log(mass/pt),wt);
        h_mpt_3[2][i-1][j-1]->Fill(log(mass/pt),wt);
      }
    }
}

f->Write();
f->Close();
return 0;

}

