							Short Instructions for MSSM HBB selection
								and appropriated classes description 
											
classes description:

BasicTree - contain basic general information specific for jet analysis. In particular, it contains:
 - information about the output TFile, output TBranches, output TTree;
 - declaration of all variables that could be used in the jet analysis;
 - deffinition of general Jet variables, like Pt, Eta...;
 - Computation of di-Jet object;
 - Calculation of Luminosity normalisation (it's basic for all analysis).
 
 
 Weights - perform calculation of all weight finctions in analysis:
  - FactorizationPtWeight, to calculate online pT trigger efficiency 1D*1D (for systematics)
  - TwoDPtWeight, to calculate online pT trigger efficiency 2D
  - dEtaWeight, to calculate online dEta Trigger Efficiency for Low M trigger (for High M trigger it's always 1)
  - BTagWeight, to calculate online BTag Trigger Efficiency
  - PileUpWeight, for PileUp reweighting 
  - HtWeight, for HT reweighting if needed (for Madgraph)
  - LumiWeight, normalise MC to data Luminosity
  
 BTagCalibrationStandalone - class that provide information about offline BTag Scale Factors from the 
 SFbLib.csv file in bin/input_corrections. Developed by official community. 
 Documentation: https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration
 
 MssmHbb - main class for MSSM HBB analysis. It provides methods to add trigger objects (by the default it's 
 for the Low M selection), perform online and offline selection. After the trigger objects have bin settuped 
 it decide whethere it's a Low M selection or high M selection and apply this knowledge to choose offline and 
 online selection. Also it provides a tools to setup weight functions and write them to the output TTree.
 By the default name of the output file will be constructed from the date and structure of my ntuples(it gives 
 a user an opportunity to not change the name of the output file at all, because it will be different for data
 and MC and for different days).
 Moreover it provides methods to calculate SFb (based on information from BTagCalibrationStandalone) and FlavourComposition (for MC).
 
 Selection scripts:
 Main selection scripts are bin/leavesDoubleBTagSelection.cc and bin/leavesTripleBTagSelection.cc. The structure 
 of this files are, in general, the same. The full description is provided inside this file. Just small tips and 
 comments will be provided there. 
  - analysis.addTriggerObjects() - by the default contain informatio nabout Low M trigger objects and the "type" of
  the selection is automatically setted up as Low M selection. If you want to make a High M selection you need to 
  specify std::vector<std::string> with trigger names yourself. Depending of the trigger objects name the type of 
  the selection will be choosen automatically. !!!! For MC you should specify selection type by yourself with:
  analysis.setLowMSelection(bool)
  
  - All correction files are in input_corrections directory and also wil lbe choosen automatically depends on 
  the selection type and provided data(Data or MC)
  
  - You can add Branches that you need explicitly with analsys.getOutputTree()->Branch("MyBranch",&my_var,"MyBranch/TYPE")
  
macros/simpleHistoBuilderExample.C - explain how to use trees in your analysis.
 
 						All suggestions and comments are strognly appreciated!
 											Have fun :)
 