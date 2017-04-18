#!/bin/sh
# $1 = mass
# creating workspace
combineTool.py -M T2W -i hbb_mbb${1}_mssm-13TeV.txt -o workspace_${1}.root
combine -M MaxLikelihoodFit --rMin=-10 --rMax=10 --robustFit 1 -m ${1} hbb_mbb${1}_mssm-13TeV.txt -t -1 --expectSignal=1
