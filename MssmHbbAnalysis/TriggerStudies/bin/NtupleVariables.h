#ifndef MssmHbbAnalysis_Ntuplizer_bin_NtupleVariables_h
#define MssmHbbAnalysis_Ntuplizer_bin_NtupleVariables_h

#include <memory>
#include <map>
#include <string>

typedef std::map<std::string, bool>::iterator it_trg;
typedef std::map<std::string, int>::iterator it_cnt;

// Variables declaration

static const int maxCands_ = 100;

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
float jetBtag_[4][maxCands_] ;


// trigger accepts
// bool * trgAccepts_;
// std::unique_ptr<bool[]> trgAccepts_;
std::map<std::string, bool> trgAccepts_;

#endif


