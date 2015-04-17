cmsDriver.py step2 \
--dasquery "file dataset=/QCD_Pt_120to170_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-8ae529a7ebd23f0b7f602aecd5540a42/USER instance=prod/phys03" \
--python_filename=QCD_Pt_120to170_bbEnriched_TuneZ2star_13TeV_pythia6_cff_py_GEN_SIM_RAW.py \
--fileout file:step2.root --pileup_input dbs:/MinBias_TuneA2MB_13TeV-pythia8/Fall13-POSTLS162_V1-v1/GEN-SIM --mc --eventcontent RAWSIM --pileup AVE_40_BX_25ns --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --conditions auto:run2_mc_GRun --step DIGI,L1,DIGI2RAW,HLT --magField 38T_PostLS1 --geometry Extended2015 --no_exec -n 10



cmsDriver.py step2 \
--python_filename=QCD_bbEnriched_TuneZ2star_13TeV_pythia6_cff_py_GEN_SIM_RAW.py \
--fileout file:step2.root --pileup_input dbs:/MinBias_TuneA2MB_13TeV-pythia8/Fall13-POSTLS162_V1-v1/GEN-SIM --mc --eventcontent RAWSIM --pileup AVE_40_BX_25ns --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --conditions auto:run2_mc_GRun --step DIGI,L1,DIGI2RAW,HLT --magField 38T_PostLS1 --geometry Extended2015 --no_exec -n 10
