#!/bin/sh

# script to run limit calculation from combination tools
# Documentation:
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit#Datacard_for_Shape_analyses
#

echo "combine -M Asymptotic -m MASS hbb_mbb*_mssm-13TeV.txt"

cd ../datacards/

limit_file=Hbb.limits
[[ -f "$limit_file" ]] && rm "$limit_file"

declare -a points=(100 120 160 200 250 300 350 400 500 600 700 900 1100 1300)

for i in "${points[@]}"; do
	name=hbb_mbb${i}_mssm-13TeV.txt
	echo "Process $i GeV Mass Point with: $name"
	combine -M Asymptotic -n Hbb -m ${i} ${name}
	root_name=`readlink -f "higgsCombineHbb.Asymptotic.mH${i}.root"`
	echo "$root_name" >> "$limit_file"	
done

echo "Merge Combination output:"

hadd -f combineMerge.root higgsCombineHbb.Asymptotic.mH*.root

echo "Merge Done: combineMerge.root were created"

echo

cd ..

echo "Done"
