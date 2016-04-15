#include "Analysis/QCDSideBand/interface/Datasets.h"

#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


Datasets::Datasets(std::string iDir) :
  idir_(iDir)
{
  

}


Datasets::~Datasets() {

}


void Datasets::readFile(std::string filename) {

  std::string line;
  std::ifstream myfile(filename);

  std::cout << "Reading dataset file :" << filename << std::endl;

  if (myfile.is_open()) {

    while ( myfile.good() ) {
      getline(myfile,line);
  
      // split line by whitespace
      std::vector<std::string> splitVec;
      boost::split(splitVec, line, boost::algorithm::is_any_of("\t\v\f\r "));
      splitVec.erase(std::remove(splitVec.begin(), 
				 splitVec.end(), 
				 ""), 
		     splitVec.end());

      if (splitVec.size() >= 2) { 
      //std::cout << splitVec.at(0) << ":" << splitVec.at(1) << ":" << splitVec.at(2) << std::endl;
	addDataset(splitVec.at(0), 
		   boost::lexical_cast<int>( splitVec.at(1) )); //, 
		   //boost::lexical_cast<double>( splitVec.at(2) ),
		   //boost::lexical_cast<int>( splitVec.at(3) ));
      }
    }

    myfile.close();

  }
  else std::cerr << "Unable to open " << filename << std::endl; 

}





void Datasets::addDataset(std::string name, int isData) {
  //  std::cout << name << "\t" << nevts << "\t" << sigma << "\t" << (isData>0?"Data":"MC") << std::endl;
  names_.push_back(name);
  //nevts_.push_back(nevts);
  //sigmas_.push_back(sigma);
  isData_.push_back(isData>0);
}


Dataset Datasets::getDataset(unsigned i) {
  Dataset tmp;
  tmp.name = names_.at(i);
  //tmp.nEvents = nevts_.at(i);
  //tmp.sigma = sigmas_.at(i);
  tmp.isData = isData_.at(i);
  return tmp;
}

Dataset Datasets::getDataset(std::string name) {
  Dataset tmp;
  for (unsigned i=0; i<names_.size(); ++i) {
    tmp.name = names_.at(i);
    //tmp.nEvents = nevts_.at(i);
    //tmp.sigma = sigmas_.at(i);
    tmp.isData = isData_.at(i);
    if (tmp.name==name) return tmp;
  }
  return Dataset();
}

TFile* Datasets::getTFile(std::string name) {

  if (idir_.size() == 0) return 0;

  TFile* ifile = TFile::Open( (idir_+std::string("/")+name+std::string(".root")).c_str(), "READ");
    
  if (ifile==0) {
    std::cerr << "No file for " << name << std::endl;
  }

  return ifile;

}




