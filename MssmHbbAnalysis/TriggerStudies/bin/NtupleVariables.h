#ifndef MssmHbbAnalysis_Ntuplizer_bin_NtupleVariables_h
#define MssmHbbAnalysis_Ntuplizer_bin_NtupleVariables_h

// Variables declaration

static const int maxCands_ = 1000;

int run_;
int lumi_;
int event_;
int bx_;
int orbit_;

// jets
int   jetN_  [4] ;
float jetPt_ [4][maxCands_] ;
float jetE_  [4][maxCands_] ;
float jetEta_[4][maxCands_] ;
float jetPhi_[4][maxCands_] ;

#endif


