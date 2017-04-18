#ifndef Analysis_TriggerStudies_L1TriggersSemiLep_h
#define Analysis_TriggerStudies_L1TriggersSemiLep_h 1

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


bool L1SingleMu3(Analysis & );


// L1 triggers

// ----------------------------------------------------------------------
bool L1SingleMu3(Analysis & analysis)
{
   if ( ! analysis.triggerResult("HLT_L1SingleMu3_v") ) return false;
   
   return true;
}

#endif  // Analysis_TriggerStudies_L1TriggersSemiLep_h
