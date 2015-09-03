#!/bin/csh -f

set ranges = {"15to20","20to30","30to50","50to80","80to120","120to170","170to300","300to470","470to600","600to800","800to1000","1000toInf"}
#set ranges = {"300to470"}
#set ranges = {"50to80"}

foreach range ( $ranges )
   rm -f rootFileList.txt
   sed -e "s/RANGE/$range/g" rootFileList.txt_temp > rootFileList.txt
   ls -1 `cat rootFileList.txt`
   TagProbeOnlineBtag $range "HLT_BTagMu_DiJet20_Mu5"  mc ptbin
   TagProbeOnlineBtag $range "HLT_BTagMu_DiJet40_Mu5"  mc ptbin
   TagProbeOnlineBtag $range "HLT_BTagMu_DiJet70_Mu5"  mc ptbin
   TagProbeOnlineBtag $range "HLT_BTagMu_DiJet110_Mu5" mc ptbin
   TagProbeOnlineBtag $range "HLT_BTagMu_Jet300_Mu5"   mc ptbin
end

exit

