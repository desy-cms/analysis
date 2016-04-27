#!/bin/csh -f

if ( $#argv < 1 ) then
   echo "Usage: srmrmdir.csh <pnfs dir>"
   exit
endif

# Get the full directory path to be removed from the command line
set pnfsDir = $1

# List of all files and directories recursively in pnfsDir
set archives = `find $pnfsDir -depth -iname "*"`

# Loop over all files and directories and remove each of them
foreach archive ( $archives ) 
   if ( -f $archive ) then
      echo Deleting file $archive ...
      srmrm "srm://dcache-se-cms.desy.de:8443/srm/managerv2?SFN=$archive"
   else if ( -d $archive ) then
      echo Deleting directory $archive ...
      srmrmdir "srm://dcache-se-cms.desy.de:8443/srm/managerv2?SFN=$archive"
   else
      echo $archive is neither file nor directory
   endif
   
end

exit
