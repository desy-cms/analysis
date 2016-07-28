#!/bin/sh

# script to run limit calculation from combination tools
# Documentation:
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit#Datacard_for_Shape_analyses
#

echo "combine -M MaxLikelihoodFit -m MASS --saveWithUncertainties --saveShapes hbb_mbb350_mssm-13TeV.txt -n NAME"

cd ../datacards/v20160721/lowM

diffNuis="/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/HiggsAnalysis/CombinedLimit/test/diffNuisances.py"
declare -a points=(100 120 160 200 250 300 350 400 500 600 700 900 1100 1300)
#declare -a points=(200 250 300 350 400 500 600 700 900 1100 1300)

for i in "${points[@]}"; do
	name=hbb_mbb${i}_mssm-13TeV.txt
	echo "Process $i GeV Mass Point with: $name"
	combine -M MaxLikelihoodFit -n Hbb_${i} -m ${i} --saveWithUncertainties --saveShapes --saveWorkspace ${name} #&> logs/${i}_bestfit.log	
	python ${diffNuis} -a -A "mlfitHbb_${i}.root" -g "NuisancePull_Hbb_${i}.root"
#	postFitPlot -r mlfitHbb_${i}.root -m ${i} -
done

echo "Done"
