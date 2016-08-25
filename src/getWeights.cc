#include "eventSelection.hh"
#include <iostream>
#include <fstream>

int main (int argc, char **argv)
{
		 if (argc < 2)
		 {
		    printf("\nUsage: %s *.root\n\n",argv[0]);
		    exit(0);
		 }
		 
		int const ind = argc - 1; 
 		TString *path = new TString[ind];
 		TFile *f_all = new TFile("metadata.root","recreate");
	    TFile *f;
	    TH1D* h_temp; 
	    TH1D* h_old;
	    char* hname;
	    ofstream outfile;
	    outfile.open("outfile.txt");

		 for ( int j = 0; j < argc-1; j++)
		 {
			path[j] = (argv[j+1]);
			ifstream file_to_read((TString)"./files/"+path[j]);
			if(file_to_read.is_open() != 1 )
			  {
			    cout << "The input file " << "./files/"<< path[j]  << " does not exist." << endl;
			    exit(1);
			  }

			  outfile << Form("//  %s \n",path[j].Data());

		 while(!file_to_read.eof())
		 	//loop through files
		   {
		     TString path_to_the_file ;
		     file_to_read >> path_to_the_file;
		     if(path_to_the_file == "") continue;
		     cout << "Processing File: " << path_to_the_file << endl;
		     f = new TFile(path_to_the_file,"read");
		     hname = (char*)(f->GetListOfKeys()->At(1)->GetName());
		     if(f_all->GetListOfKeys()->Contains(hname)){
		     	h_old = (TH1D*)f_all->Get(hname);
		     	h_old->Add((TH1D*)f->Get(hname));
		     	f_all->cd();
		     	h_old->Write("",TObject::kOverwrite);
		     }
		     else{
		     	h_temp = (TH1D*)f->Get(hname);
		     	f_all->cd();
		     	h_temp->Write();	
		     	cout << "Histo " << hname << " added" << endl;
		     }
		     f->Close();
		    }
		    TList* KeyList = f_all->GetListOfKeys();
		    for(int i = 0; i<KeyList->GetSize(); i++){
		    	hname = (char*)(KeyList->At(i)->GetName());
		    	h_temp = (TH1D*)f_all->Get(hname);
		    	outfile << Form("if(Run == %.*s) Nevent = %3.1f; \n",6,hname+20,h_temp->GetBinContent(1));
		    	//outfile << Form("initSumWeights = %f; \n} \n",h_temp->GetBinContent(3));
		    }


		   }
		   f_all->Close();
		   return 0;

		 }
