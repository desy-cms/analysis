#ifndef Analysis_Ntuplizer_bin_NtupleVariables_h
#define Analysis_Ntuplizer_bin_NtupleVariables_h

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

double xsection_;

// jets
int   jetN_  [4] ;
float jetPt_ [4][maxCands_] ;
float jetE_  [4][maxCands_] ;
float jetEt_ [4][maxCands_] ;
float jetEta_[4][maxCands_] ;
float jetPhi_[4][maxCands_] ;
int   jetBtagN_[4] ;
float jetBtag_ [4][maxCands_] ;

// vertices
int   pvN_[4];
float pvX_[4][maxCands_];
float pvY_[4][maxCands_];
float pvZ_[4][maxCands_];
float pvXe_[4][maxCands_];
float pvYe_[4][maxCands_];
float pvZe_[4][maxCands_];
bool  pvFake_[4][maxCands_];
float pvChi2_[4][maxCands_];
float pvNdof_[4][maxCands_];
float pvRho_[4][maxCands_];

// trigger accepts
// bool * trgAccepts_;
// std::unique_ptr<bool[]> trgAccepts_;
std::map<std::string, bool> trgAccepts_;

#endif



