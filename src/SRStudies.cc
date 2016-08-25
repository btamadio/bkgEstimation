#include "SRStudies.hh"
int main (int argc, char **argv){

  if (argc < 2)
    {
      printf("\nUsage: %s *.root\n\n",argv[0]);
      exit(0);
    }
  
  int const ind = argc - 1;
  TString *path = new TString[ind];
  TChain *chain = new TChain("miniTree");

       for ( int j = 0; j < argc-1; j++)
       {
         path[j] = (argv[j+1]); 
         chain->Add(path[j]);
       }

  p = new miniTree(chain);
  
  TFile *f = new TFile("RPV_kin.root", "recreate");
  f->cd();
  
  TH2F *h_phi_eta = new TH2F("h_phi_eta","h_phi_eta",15,-3.14,3.14,20,-2,2);
  //TH1F *

  //initialize loop
  int numev = p->fChain->GetEntries();
  cout << " Total number of events " << numev << endl;

  //event loop
  for( int nEvent = 0 ; nEvent < numev ; nEvent ++ ){  
    get_event(nEvent);
    if( nEvent%10000 == 0 ) cout << nEvent << " events processed " << endl;

    double wt = p->weight; 
    int njet = p->njet; 
    //int btags = p->nbjet_ak4;
    //int b_tag = (int)((bool)btags);
    double dEta_cut = 1.4;

    bool interesting  = (njet >= 5 && p->MJ > 0.6);
    if(!interesting) continue;

    bool isVR = (p->dEta > dEta_cut);
    isVR = (int)isVR;
    int bin;
    for(int k = 0; k < njet; k++){
      bin = h_phi_eta->Fill((*p->jet_phi)[k],(*p->jet_eta)[k],wt);
    }

    if(p->runNumber == 300863){
      cout << "Run: " << p->runNumber << endl;
      cout << "Event: " << p->eventNumber << endl;
      cout << "LB: " << p->lumiBlock << endl;
      //cout << "njet_ak4: " << p->njet_Fix70 << endl;
      cout << "njet: " << p->njet << endl;
      cout << "MJ: " << p->MJ << endl;
      cout << "Jet 1 info: " << endl;
      cout << " pt: " << (*p->jet_pt)[0] << endl;
      cout << " eta: " << (*p->jet_eta)[0] << endl;
      cout << " phi: " << (*p->jet_phi)[0] << endl;
      cout << endl;

    }
      


}

f->Write();
f->Close();
return 0;

}

