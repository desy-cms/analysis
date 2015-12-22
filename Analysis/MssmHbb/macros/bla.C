// This tutorial illustrates how to:
//   - create two simple TTrees
//   - Create a Tree Index
//   - Make a friend TTree
//   - compare two TTrees
//   - Draw a variable from the first tree versus a variable
//     in the friend Tree
//
// You can run this tutorial with:
//  root > .L treefriend_modified.C+
//  root > CreateFriendTree();
//  root > CompareTrees();
//  root > DrawFriend();
//
//  Original Author: Rene Brun (root/tutorial/treefriend.C)

//  modified by K.Hoshina

      
#include "TTree.h"
#include "TFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TROOT.h"
#include <vector>
#include <iostream>

class Pos {
 public:
   Pos(){}
   float x;
   float y;
   float z;
};

class Hit {
 public:
   Hit(){}
   float time_;
   int   number_;
};


#ifdef __MAKECINT__
#pragma link C++ class Pos+;
#pragma link C++ class vector<Hit>+;
#endif

Int_t Run, Event;
Pos *start, *stop, *fstart, *fstop;
vector<Hit> *hitvec0;
vector<Hit> *hitvec1;
vector<Hit> *fhitvec0;
vector<Hit> *fhitvec1;
   
void CreateMainTree() {

   // create parent tree
   TFile *f = new TFile("tree.root","recreate");
   TTree *T = new TTree("T","test tree");

   // Two branches ("Run" and "Event") will be used to index the Tree
   T->Branch("Run",&Run,"Run/I");
   T->Branch("Event",&Event,"Event/I");

   //
   // Make branches of objects
   // 
   // Arguments of Bronch function is ("branchname", "typename", pointer to buffer)
   //
   // 1) You must give identical name for "branchname" within ALL friend trees
   //    ONLY WHEN you want to use TTreeViewer from main tree.
   //    e.g. <treename>.<typename>_id..
   //    (CAUTION: branchname must be identical within one tree.)
   //    You may need root v5.17.00 or later to see all friend trees in TTreeViewer.
   // 
   // 2) The "branchname" must include one dot "." in the end of the name
   //    to identify two positions.
   //    (Just try to remove last dot and see what will happen)
   //

   // case 1: normal object branch
   // add two positions, start and stop.
 
   start = new Pos();
   stop = new Pos();
   T->Bronch("T.start.","Pos",&start); 
   T->Bronch("T.stop.","Pos",&stop); 
   //T->Bronch("start","Pos",&start); // wrong! start & stop branch will be identical
   //T->Bronch("stop","Pos",&stop);   //

   // case 2: vector branch
   // add vectors of Hit. 
   hitvec0 = new vector<Hit>;
   hitvec1 = new vector<Hit>;
   T->Bronch("T.vector<Hit>_0.","vector<Hit>",&hitvec0);
   T->Bronch("T.vector<Hit>_1.","vector<Hit>",&hitvec1);
   //T->Bronch("T.vector<Hit>_0","vector<Hit>",&hitvec0); // it's OK in the case of vector...
   //T->Bronch("T.vector<Hit>_1","vector<Hit>",&hitvec1); // 

   TRandom r;
   for (Int_t i=0;i<10000;i++) {
      if (i < 5000) Run = 1;
      else          Run = 2;
      Event = i;
      start->x = r.Gaus(10,1);
      start->y = r.Gaus(20,2);
      start->z = r.Landau(2,1);

      stop->x = start->x + 100.;
      stop->y = start->y + 100.;
      stop->z = start->z + 100.;

      int nhits = (int)(r.Uniform(0.1,1)*10);
      hitvec0->resize(nhits);
      hitvec1->resize(nhits);
      
      for (Int_t j=0; j<nhits; j++) {
         float t = r.Gaus(30, 1);
         Hit h;
         h.time_ = t;
         h.number_ = j;
         (*hitvec0)[j] = h;

         float t2= r.Gaus(130, 1);
         h.time_ = t2;
         (*hitvec1)[j] = h;
      }
      T->Fill();
   }

   f->Write();
   f->Close();
   delete f;
}

void CreateFriendTree() {

   // open main tree root file
   TFile *f = new TFile("tree.root","update");
   TTree *T = (TTree*)gROOT->FindObject("T");

   Int_t fRun,fEvent;
   fstart = new Pos();
   fstop = new Pos();
   fhitvec0 = new vector<Hit>;
   fhitvec1 = new vector<Hit>;
   T->SetBranchAddress("Run",&Run);
   T->SetBranchAddress("Event",&Event);
   T->SetBranchAddress("T.start.",&start);
   T->SetBranchAddress("T.stop.",&stop);

   // CAUTION:
   // You must remove last dot to SetBranchAddress 
   // only in the case of vector...
   // It's not consistent, but required from ROOT.
   // Just check the name of branch by TBrowser.
   T->SetBranchAddress("T.vector<Hit>_0",&hitvec0);
   T->SetBranchAddress("T.vector<Hit>_1",&hitvec1);

   // create friend tree
   TTree *TF = new TTree("TF","friend tree");

   TF->Branch("Run",&fRun,"Run/I");
   TF->Branch("Event",&fEvent,"Event/I");
   TF->Bronch("TF.start.","Pos",&fstart); 
   TF->Bronch("TF.stop.","Pos",&fstop); 
   TF->Bronch("TF.vector<Hit>_0.","vector<Hit>",&fhitvec0);
   TF->Bronch("TF.vector<Hit>_1.","vector<Hit>",&fhitvec1);
 
   Int_t nentries = (Int_t)T->GetEntries();
   for (Int_t i=0;i<nentries;i++) {
      T->GetEntry(i);

      // skip some events...
      if (start->z >5) continue;

      fRun = Run;
      fEvent = Event;
      fstart->x = start->x + 1000;
      fstart->y = start->y + 1000;
      fstart->z = start->z + 1000;
      fstop->x = stop->x + 1000;
      fstop->y = stop->y + 1000;
      fstop->z = stop->z + 1000;

      fhitvec0->resize(hitvec0->size());
      fhitvec1->resize(hitvec0->size());
      for (UInt_t j=0; j<hitvec0->size(); j++) {
         (*fhitvec0)[j] = (*hitvec0)[j];
         (*fhitvec0)[j].time_ += 1000;
         (*fhitvec1)[j] = (*hitvec1)[j];
         (*fhitvec1)[j].time_ += 1000;
      }

      TF->Fill();
   }

   //
   // Add friend tree to main tree.
   //
   // First, you have to give index keys

   TF->BuildIndex("Run","Event");

   // Then add it to main tree...
   T->AddFriend(TF);

   // delete old tree (which is NOT assigned any friend tree)
   f->Delete("T;1");

   f->Write();
   f->Close();
   delete f;
}


void CompareTrees() {
   // The two TTrees created above are compared.
   // The subset of entries in the small TTree must be identical
   // to the entries in the original TTree.
   
   TFile *f = new TFile("tree.root");
   TTree *T  = (TTree*)f->Get("T");
   TTree *TF = (TTree*)f->Get("TF");
   Int_t fRun,fEvent;
   T->SetBranchAddress("Run",&Run);
   T->SetBranchAddress("Event",&Event);
   T->SetBranchAddress("T.start.",&start);
   T->SetBranchAddress("T.stop.",&stop);
   T->SetBranchAddress("T.vector<Hit>_0",&hitvec0);
   T->SetBranchAddress("T.vector<Hit>_1",&hitvec1);

   TF->SetBranchAddress("Run",&fRun);
   TF->SetBranchAddress("Event",&fEvent);
   TF->SetBranchAddress("TF.start.",&fstart);
   TF->SetBranchAddress("TF.stop.",&fstop);
   TF->SetBranchAddress("TF.vector<Hit>_0",&fhitvec0);
   TF->SetBranchAddress("TF.vector<Hit>_1",&fhitvec1);

   //
   // Oops!!!
   // This line seems to be essentially important
   // to get correct behavior of T->GetEntry(i)!
   // you MUST call AddFriend() even if we already added friend tree
   // inside CreateFriend() function...
   //
   T->AddFriend(TF);
  
   Int_t nentries = (Int_t)T->GetEntries();
   cerr << "Nentries of main tree is " << nentries << endl;

   Int_t nok = 0;
   for (Int_t i=0;i<nentries;i++) {
      T->GetEntry(i);
      if (fRun == Run && fEvent==Event) {
         printf("Key matched i=%d, Run=%d Event=%d\n", i, Run, Event);
         nok++;

         int nhits = (int)hitvec0->size();
         int fnhits = (int)fhitvec0->size();
         if (nhits != fnhits) {
            cerr << " nhits doesn't match?? " << endl;

         } else {
            for (Int_t j=0; j<nhits; j++) {
                printf("   T.hitvec[%d] = %f, TF.hitvec[%d] = %f\n",
                       j, (*hitvec0)[j].time_, j, (*fhitvec0)[j].time_);
            }
         }

      } else {
         printf("Key doesn't match: i=%d, Run=%d, Event=%d, fRun=%d, fEvent=%d\n",i, Run, Event, fRun, fEvent);
      } 
   }
   printf("number of matched = %d, Nentries of TF=%lld\n",nok,TF->GetEntries());

   delete f;
}

void DrawFriend() {
  // Draw a scatter plot of variable x in the parent TTree versus
  // the same variable in the subtree.
  // This should produce points along a straight line.
   
   TFile *f  = TFile::Open("tree.root");
   TTree *T  = (TTree*)f->Get("T");
   
   // for Draw(), we don't need to call AddFriend, somehow...
   T->Draw("T.vector<Hit>_0.time_:TF.vector<Hit>_0.time_");

   //
   // open TTreeViewer from main tree
   // root v5.10.00 doesn't show friends in TTreeViewer...
   // use v5.17.00 or later. 
   T->StartViewer();

}

void bla() {
   CreateMainTree();
   CreateFriendTree();
   CompareTrees();
   DrawFriend();
}

