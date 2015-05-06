#!/bin/csh -f

set files = `/bin/ls -1 /nfs/dust/cms/user/walsh/analysis/higgs/generated/background/ntuple_qcd*.root`

foreach file ( $files )
   rm -f rootFileList.txt
   echo $file > rootFileList.txt
   GeneratorBackgroundAnalysis
end

set cuts = `ls -1tr genjets_qcd*.root | tail -n 1 | awk -F "." '{print $2}'`

echo $cuts
hadd hadd_genjets_qcd.$cuts.root genjets_qcd*.$cuts.root

if ( ! -d $cuts ) then
   mkdir $cuts
endif
mv genjets_qcd*.$cuts.root $cuts

exit
