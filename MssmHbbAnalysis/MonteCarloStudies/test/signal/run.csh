#!/bin/csh -f

@ mw = $8
@ dmw = $9

cd m300
if ( -e rootFileList.txt ) then
   rm -f rootFileList.txt
endif
/bin/ls -1 /nfs/dust/cms/user/walsh/analysis/higgs/generated/signal/m300/ntuple_ak4genjets_*.root > rootFileList.txt
if ( $mw > 0 ) then
   @ mw = 300
   @ dmw = 60
endif
GeneratorBackgroundAnalysis $1 $2 $3 $4 $5 $6 $7 $mw $dmw


cd ../m600
if ( -e rootFileList.txt ) then
   rm -f rootFileList.txt
endif
/bin/ls -1 /nfs/dust/cms/user/walsh/analysis/higgs/generated/signal/m600/ntuple_ak4genjets_*.root > rootFileList.txt
if ( $mw > 0 ) then
   @ mw = 500
   @ dmw = 120
endif
GeneratorBackgroundAnalysis $1 $2 $3 $4 $5 $6 $7 $mw $dmw


cd ../m1100
if ( -e rootFileList.txt ) then
   rm -f rootFileList.txt
endif
/bin/ls -1 /nfs/dust/cms/user/walsh/analysis/higgs/generated/signal/m1100/ntuple_ak4genjets_*.root > rootFileList.txt
if ( $mw > 0 ) then
   @ mw = 700
   @ dmw = 300
endif
GeneratorBackgroundAnalysis $1 $2 $3 $4 $5 $6 $7 $mw $dmw 
cd ..

exit
