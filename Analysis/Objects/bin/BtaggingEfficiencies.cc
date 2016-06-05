
#include "Analysis/Objects/interface/Btagging.h"

using namespace analysis::objects;


// =============================================================================================   
int main(int argc, char * argv[])
{
   // Intantiation of btagging object
   Btagging btagging("rootFileList.txt","MssmHbb/Events/EventInfo");

// COLLECTIONS
   // Jet stuff
   btagging.jetsCollection("MssmHbb/Events/slimmedJetsReapplyJEC"); 
   // GenParticles (when needed)
   btagging.genParticlesCollection("MssmHbb/Events/prunedGenParticles"); 
   // Cross sections
   btagging.crossSections("MssmHbb/Metadata/CrossSections");
   
   // ------
   btagging.workingPoint(0.935);
//   btagging.flavourDefinition();  // default "Hadron"
//   btagging.btagAlgorithm();      // default "btag_csvivf"
   btagging.ptMin();               // default  20. (GeV)
   btagging.etaMax();              // default  2.5
   btagging.flavourDefinition("Extended");
   btagging.extendedFlavour();    // default rmax = 0.4, ptmin = 5.
   
   btagging.scaleLuminosity(10000.);
   
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

