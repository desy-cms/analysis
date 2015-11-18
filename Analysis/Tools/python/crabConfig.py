from WMCore.Configuration import Configuration

# This routine contains default settings for a crab configuration.
# Used in a main script the default values can be changed accordingly.
# The idea is to avoid a large main script.

def crabConfig():
   config = Configuration()
   
   # -- General
   config.section_('General')
   config.General.transferOutputs = True
   config.General.requestName     = ''
   config.General.workArea        = 'crab_projects'
   
   # -- JobType
   config.section_('JobType')
   config.JobType.psetName    = 'ntuplizer.py'
   config.JobType.pluginName  = 'Analysis'
   config.JobType.outputFiles = ['ntuple.root']
   
   # -- Data
   config.section_('Data')
   config.Data.publication  = False
   config.Data.unitsPerJob  = 1
   config.Data.totalUnits   = -1
   config.Data.splitting    = 'FileBased'
   config.Data.outLFNDirBase= ''
   
   # -- User
   config.section_('User')
   config.User.voGroup = 'dcms'
   
   # -- Site
   config.section_('Site')
   config.Site.storageSite = 'T2_DE_DESY'
   
   return config
