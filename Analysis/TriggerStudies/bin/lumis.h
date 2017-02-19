#include <map>

#include "TTree.h" 

struct brilcalc
{
   std::map<int, std::map<int, float> > lumiByLS;
   std::map<int, std::map<int, float> > pileupByLS;
};

std::map<int, std::map<int, float> > lumiByLS;

brilcalc readLumisCsv(const std::string & csv);

brilcalc readLumisCsv(const std::string & csv)
{
   brilcalc bc;

   int run;
   int fill;
   int ls;
   float delivered;
   float recorded;
   float pileup;
   
   TTree *T = new TTree("ntuple","data from csv file");
   T->ReadFile(csv.c_str(),"run/I:fill/I:ls/I:deliv/F:record/F:pileup/F",',');
   
   T->SetBranchAddress("run"   ,&run);
   T->SetBranchAddress("fill"  ,&fill);
   T->SetBranchAddress("ls"    ,&ls);
   T->SetBranchAddress("deliv" ,&delivered);
   T->SetBranchAddress("record",&recorded);
   T->SetBranchAddress("pileup",&pileup);
   
   for ( int i = 0 ; i < T->GetEntries() ; ++i )
   {
      T -> GetEntry(i);
      bc.lumiByLS[run][ls] = recorded;
      bc.pileupByLS[run][ls] = pileup;
   }
   
   return bc;
}
