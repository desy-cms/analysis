#ifndef Analysis_Ntuplizer_bin_NtupleVariables_h
#define Analysis_Ntuplizer_bin_NtupleVariables_h

#include <memory>
#include <map>
#include <string>

#include "TLorentzVector.h"

typedef std::map<std::string, bool>::iterator it_trg;
typedef std::map<std::string, int>::iterator it_cnt;

// Variables declaration

static const int maxCands_ = 30;
static const int maxGenCands_ = 2000;

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

struct PatJet {
  TLorentzVector p;
  float btag;
  int flavour;
  bool looseId;
  bool tightId;
};

std::map<std::string, float*> fPatJets_;
std::map<std::string, int(*)[maxCands_] >   iPatJets_;

// jet branches
int   jetN_   ;
float jetPt_  [maxCands_];
float jetE_   [maxCands_];
float jetEt_  [maxCands_];
float jetEta_ [maxCands_];
float jetPhi_ [maxCands_];
float jetBtag_[maxCands_];
int   jetFlav_[maxCands_];
float jetNHadFrac_[maxCands_];
float jetNEmFrac_ [maxCands_];
float jetNMult_   [maxCands_];
float jetCHadFrac_[maxCands_];
float jetCEmFrac_ [maxCands_];
float jetCMult_   [maxCands_];
float jetMuFrac_  [maxCands_];

// jet collections
PatJet jets_[maxCands_];

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



