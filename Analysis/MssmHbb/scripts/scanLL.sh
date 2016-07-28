#!/bin/sh

# script to run limit calculation from combination tools
# Documentation:
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit#Datacard_for_Shape_analyses
#

echo "combine -M Asymptotic -m MASS hbb_mbb*_mssm-13TeV.txt"

cd ../datacards/Scan
dc_folder="../finale/combination/"

declare -a points=(200 250 300 350 400 500 600 700 900 1100 1300)
declare -a syst=("CMS_JES_13TeV" "CMS_JER_13TeV" "CMS_PU_13TeV" "CMS_PtEff_13TeV" "CMS_SFl_13TeV" "CMS_SFb_13TeV" "CMS_PAR0_13TeV" "CMS_PAR1_13TeV" "CMS_PAR2_13TeV" "CMS_lumi_13TeV" "CMS_onlBtag_13TeV")
declare -a group=("Bg" "JE" "Stat")

for i in "${points[@]}"; do
	name="${dc_folder}hbb_mbb${i}_mssm-13TeV.txt"
	echo "Process $i GeV Mass Point with: $name"
	for s in "${syst[@]}"; do
		echo "Freeze nuisance parameter: $s"
		echo "Process: combine -M Asymptotic -n HbbScan_${s} -m ${i} ${name} --freezeNuisances ${s}"
		combine -M Asymptotic -n HbbScan_${s} -m ${i} ${name} --freezeNuisances ${s}
	done	
	for sg in "${group[@]}"; do
                echo "Freeze group of nuisance parameters: $sg"
                echo "Process: combine -M Asymptotic -n HbbScan_${sg} -m ${i} ${name} --freezeNuisanceGroups ${sg}"
                combine -M Asymptotic -n HbbScan_${sg} -m ${i} ${name} --freezeNuisanceGroups ${sg}
        done
done

echo "\n Make Hbb.limits lists"
for s in "${syst[@]}"; do
	limit_file="Hbb.limits.Scan_${s}"
	[[ -f "$limit_file" ]] && rm "$limit_file"
	for i in "${points[@]}"; do
		root_name=`readlink -f "higgsCombineHbbScan_${s}.Asymptotic.mH${i}.root"`
		echo "$root_name" >> "$limit_file"
	done
	echo "Limits list ${limit_file} were created"
	echo "Run PlotLimit macro"
	PlotLimit -i ${limit_file} -o ${limit_file}
done

for s in "${group[@]}"; do
        limit_file="Hbb.limits.Scan_group_${s}"
        [[ -f "$limit_file" ]] && rm "$limit_file"
        for i in "${points[@]}"; do
                root_name=`readlink -f "higgsCombineHbbScan_${s}.Asymptotic.mH${i}.root"`
                echo "$root_name" >> "$limit_file"
        done
        echo "Limits list ${limit_file} were created"
        echo "Run PlotLimit macro"
        PlotLimit -i ${limit_file} -o ${limit_file}
done

cd ../..

echo "Done"
