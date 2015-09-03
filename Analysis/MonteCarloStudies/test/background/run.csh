#!/bin/csh -f

set mypaths = `/bin/ls -1d /nfs/dust/cms/user/walsh/analysis/higgs/generated/background/qcd_*`

@ count = 0
set mydirs = ""
set roofilelist = rootFileList.txt
foreach mypath ( $mypaths )
   set mydir = `basename $mypath`
   if ( ! -d $mydir ) then
      mkdir $mydir
   endif
   cd $mydir
   if ( -e $roofilelist ) then
      rm -f $roofilelist
   endif
   @ nfiles = `/bin/ls -1 $mypath | grep ntuple_ak4genjets_ | grep root | wc -l`
   if ( nfiles != 0 ) then
      /bin/ls -1 $mypath/ntuple_ak4genjets_*.root > $roofilelist
      echo $mydir
      GeneratorBackgroundAnalysis $1 $2 $3 $4 $5 $6 $7 $8 $9
      if ( $count == 0 ) then
         @ count = 1
         set rootfile = `/bin/ls -1tr *.root | tail -n 1`
      endif
      set mydirs = "$mydirs $mydir/$rootfile"
   endif
   cd ..
end

set haddfile = "hadd_$rootfile"
if ( -e $haddfile ) then
   rm -f $haddfile
endif
hadd $haddfile $mydirs

# foreach file ( $files )
#    rm -f rootFileList.txt
#    echo $file > rootFileList.txt
#    GeneratorBackgroundAnalysis
# end
# 
# set cuts = `ls -1tr genjets_qcd*.root | tail -n 1 | awk -F "." '{print $2}'`
# 
# echo $cuts
# hadd hadd_genjets_qcd.$cuts.root genjets_qcd*.$cuts.root
# 
# if ( ! -d $cuts ) then
#    mkdir $cuts
# endif
# mv genjets_qcd*.$cuts.root $cuts
# 
exit
