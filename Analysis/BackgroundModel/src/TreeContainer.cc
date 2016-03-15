#include "TFile.h"
#include "TROOT.h"
#include "Analysis/BackgroundModel/interface/TreeContainer.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


using namespace analysis::backgroundmodel;


TreeContainer::TreeContainer(const std::string& input) :
  treeFileName_(input),
  data_(getTree_("data")) {
}


TreeContainer::~TreeContainer() = default;


std::unique_ptr<TTree> TreeContainer::data() const {
  return uniqueClone_(*data_);
}


void TreeContainer::show() const {
  std::cout << "Data events: " << data_->GetEntriesFast() << std::endl;
}


std::unique_ptr<TTree> TreeContainer::getTree_(const std::string& name) const {
  TFile file(treeFileName_.c_str(), "read");
  TTree& input = // FIXME: tree name currently hard-coded
    *static_cast<TTree*>(file.Get("MssmHbb_13TeV")->Clone(name.c_str()));
  std::unique_ptr<TTree> tree(input.CloneTree(0));
  tree->SetDirectory(0);
  tree->CopyEntries(&input);
  file.Close();
  return tree;
}


std::unique_ptr<TTree> TreeContainer::uniqueClone_(const TTree& original) {
  std::unique_ptr<TTree> clone(static_cast<TTree*>(uniqueClone(&original)));
  if (clone != nullptr) clone->SetDirectory(0);
  return clone;
}
