#ifndef Analysis_TriggerStudies_HLTPathsAllHad_h
#define Analysis_TriggerStudies_HLTPathsAllHad_h 1


#include "L1TriggersAllHad.h"

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


bool LowMassAllHad(Analysis & );
bool LowMassAllHad2017(Analysis & );


// HLT paths
// ----------------------------------------------------------------------
bool LowMassAllHad(Analysis & analysis)
{
   if ( ! analysis.triggerResult("HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v") ) return false;

   // online b-tagging trigger objects (this part was partly missing from original path above)
   // at most 6 jets with pT > 80 GeV
   auto hlt8bJets30 = analysis.collection<TriggerObject>("hltBTagCaloCSVp014DoubleWithMatching");
   std::vector<TriggerObject> hlt6bJets80;
   for ( int j = 0 ; j < hlt8bJets30->size() ; ++j )
   {
      if ( j == 6 ) break; // at most 6 jets
      TriggerObject jet = hlt8bJets30->at(j);
      if ( jet.pt() < 80. ) continue;
      hlt6bJets80.push_back(jet);
   }
   if ( hlt6bJets80.size() < 2 )                                                                         return false;
   
   return true;
}

bool LowMassAllHad2017(Analysis & analysis)
{
   if ( ! L1DoubleJet100Eta2p3_dEtaMax1p6(analysis) )                                                    return false;
   if ( ! LowMassAllHad(analysis) )                                                                      return false;
   
   // pseudorapidity cut 2.3
   auto hltdipf100 = analysis.collection<TriggerObject>("hltDoublePFJetsC100");
   std::vector<TriggerObject> hltdipf100eta2p3;
   for ( int j = 0 ; j < hltdipf100->size() ; ++j )
   {
      TriggerObject jet = hltdipf100->at(j);
      if ( fabs(jet.eta()) > 2.3 ) continue;
      hltdipf100eta2p3.push_back(jet);
   }
   if ( hltdipf100eta2p3.size() < 2 )                                                                    return false;
   
   // delta_eta cut 1.6
   bool deta = false;
   for ( int j1 = 0 ; j1 < (int)hltdipf100eta2p3.size()-1 ; ++j1 )
   {
      for ( int j2 = j1+1 ; j2 < (int)hltdipf100eta2p3.size() ; ++j2 )
      {
         float eta1 = hltdipf100eta2p3.at(j1).eta();
         float eta2 = hltdipf100eta2p3.at(j2).eta();
         if ( fabs(eta1-eta2) < 1.6 ) deta = true;
      }
   }
   if ( ! deta )                                                                                         return false;
   
   return true;
}

#endif  // Analysis_TriggerStudies_HLTPathsAllHad_h
