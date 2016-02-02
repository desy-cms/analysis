
#include "Analysis/Objects/interface/Btagging.h"

using namespace analysis::objects;


// =============================================================================================   
int main(int argc, char * argv[])
{
   // Intantiation of btagging object
   Btagging btagging("rootFileList.txt","MssmHbb/Events/EventInfo");

   // Jet stuff
   btagging.jetsCollection("Jets", "MssmHbb/Events/slimmedJetsPuppi"); 
   btagging.workingPoint(0.941);
   btagging.flavourDefinition("Hadron"); // default
   btagging.ptMin(20.);                  // default
   btagging.etaMax(2.5);                 // default
   
   // Histograms binning
   const static int nptbins = 17;
   float ptbins[nptbins+1] = {0,20,40,60,80,100,120,140,160,180,200,240,280,320,400,500,700,1000};
   const static int netabins = 4;
   float etabins[nptbins+1] = {0,1.4,1.7,2.1,2.5};
   btagging.ptBinning(nptbins,ptbins);
   btagging.etaBinning(netabins,etabins);

   // Main routine   
   btagging.efficiencies();
   
}

