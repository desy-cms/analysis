Short Instructions for Basic Jet Analysis selection (including Mssm bbH->bbbb) and appropriated classes description 

HOW TO:
1)compile your project with scram b -j4
2)Check AllInOneTool -h for available options (NOT ALL ARE INCLUDED FOR DAS, only 2bjet and datavsmc is ready,
right now, everything else can be added if needed)
3)run AllInOneTool from Analysis/MssmHbb/test with command line arguments or with "-c config.cfg" to get arguments
from config file
4)find your ouput in Analysis/MssmHbb/output/ folder
5)If values of cuts/weights/selection mechanism etc should be changed - check Analysis/MssmHbb/src/JetAnalysisBase.cpp
and Analysis/MssmHbb/src/DataMcComparison.cpp Analysis/MssmHbb/src/selectionDoubleB.cpp .

Classes description:

Inheritance Tree:

		JetAnalysisBase
			|
		selectionDoubleB
		|		|
	DataMcComparison   MssmHbbSignal

Tree is not completed.
JetAnalysisBase - main class to perform jet analysis. It declare virtual methods i.e:
-loadCorrections
-assignweights
-setupAnalysis
-applySelection
etc
this methods is overwritten in derived classes to perform more specific selection/corrections/weights etc.

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
 
Cut and CutFlow - classes that are used to perform automatic cut & flow.

Histogrmas - class that declare and initialize map of histograms.
 
All suggestions and comments are strognly appreciated! Have fun :)
 
