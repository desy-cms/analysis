#!/bin/sh
# $1 = mass
# $2 = directory 
# creating workspace
cd ${2}
combineTool.py -M T2W -i hbb_mbb${1}_mssm-13TeV.txt -o workspace_${1}.root
# computing impacts 
combineTool.py -M Impacts -d workspace_${1}.root -m $1 --doInitialFit --allPars --robustFit 1 -t -1 --rMin -10 --rMax 10 --expectSignal=1 -v5 > log_InitialFit_${1}.o
combineTool.py -M Impacts -d workspace_${1}.root -m $1 --robustFit 1 --doFits --allPars -t -1 --rMin -10 --rMax 10 --expectSignal=1 -v5 > log_robustFit_${1}.o
combineTool.py -M Impacts -d workspace_${1}.root -m $1 --allPars -o impacts_${1}.json -v5 > log_allPars_${1}.o
# plotting impacts
plotImpacts.py -i impacts_${1}.json -o impacts_$1
