#!/usr/csh
module use -a /afs/desy.de/group/cms/modulefiles/
module load cmssw
setenv SCRAM_ARCH slc6_amd64_gcc491
source /cvmfs/cms.cern.ch/cmsset_default.csh
cmsenv
voms-proxy-init -voms cms:/cms/dcms -valid 96:00

