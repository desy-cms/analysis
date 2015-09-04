#ifndef Analysis_Tools_bin_NtupleVariables_h
#define Analysis_Tools_bin_NtupleVariables_h

#include <memory>
#include <map>
#include <string>

typedef std::map<std::string, bool>::iterator it_trg;
typedef std::map<std::string, int>::iterator it_cnt;

// Variables declaration

static const int maxCands_ = 100;

// Metadata
double xsection_;
unsigned int nGenTotal_;
unsigned int nGenFilt_;

// EventInfo
int run_;
int lumi_;
int event_;
int bx_;
int orbit_;


// jets
int   jetN_   ;
float jetPt_  [maxCands_];
float jetE_   [maxCands_];
float jetEt_  [maxCands_];
float jetEta_ [maxCands_];
float jetPhi_ [maxCands_];
float jetBtag_[maxCands_];
int   jetFlav_[maxCands_];

// muons
int   muN_   ;
float muPt_ [maxCands_];
float muE_  [maxCands_];
float muEt_ [maxCands_];
float muEta_[maxCands_];
float muPhi_[maxCands_];
float muQ_  [maxCands_];


// trigger accepts
// bool * trgAccepts_;
// std::unique_ptr<bool[]> trgAccepts_;
std::map<std::string, bool> trgAccepts_;

#endif



