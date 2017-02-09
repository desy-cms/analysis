#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "TFile.h" 

#include "TH1.h" 

std::map<std::string, TH1F*> h1_;

// =============================================================================================   
int main(int argc, char * argv[])
{
   std::cout << " oi " << std::endl;
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

   h1_["nSelected"] = new TH1F("h_nSelected","",80,0,80);
   h1_["nTotal"]    = new TH1F("h_nTotal","",80,0,80);
   
   std::cout << " oioi " << std::endl;
   
   TFile * f_out = new TFile("tests.root","RECREATE");
   std::cout << " oioioi " << std::endl;
//   for ( auto & h : h1_ )
   for (std::map<std::string, TH1F*>::iterator h=h1_.begin(); h!=h1_.end(); ++h)
   {
      std::cout << " oioioi " << h->first << std::endl;
      h->second->Write();
   }
   
   f_out -> Close();
   
  
}
