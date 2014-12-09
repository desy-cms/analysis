#ifndef Analysis_Ntuplizer_bin_NtupleVariables_h
#define Analysis_Ntuplizer_bin_NtupleVariables_h

// Variables declaration

static const int maxCands_ = 1000;

int run_;
int lumi_;
int event_;
int bx_;
int orbit_;

// L1 jets
int   l1JetN_ ;
float l1JetPt_  [maxCands_] ;
float l1JetE_   [maxCands_] ;
float l1JetEta_ [maxCands_] ;
float l1JetPhi_ [maxCands_] ;
float l1JetPx_  [maxCands_] ;
float l1JetPy_  [maxCands_] ;
float l1JetPz_  [maxCands_] ;

// L2 jets
int   l2JetN_ ;
float l2JetPt_  [maxCands_] ;
float l2JetE_   [maxCands_] ;
float l2JetEta_ [maxCands_] ;
float l2JetPhi_ [maxCands_] ;
float l2JetPx_  [maxCands_] ;
float l2JetPy_  [maxCands_] ;
float l2JetPz_  [maxCands_] ;


#endif


