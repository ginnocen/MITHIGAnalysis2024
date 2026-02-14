#file path will be:
#outLFNDirBase/inputDataset/requestName/time_tag/...
from CRABClient.UserUtilities import config
config = config()
config.General.transferOutputs = True
config.General.requestName = 'g2qqbar_MC_sample_HERWIG'

config.JobType.psetName = 'recipe5_MC.py'
config.JobType.pluginName = 'Analysis'
config.JobType.allowUndistributedCMSSW = True
#config.Data.inputDataset = '/QCD_pThat-15_Dijet_TuneCP5_5p02TeV-pythia8/RunIISummer20UL17pp5TeVMiniAODv2-106X_mc2017_realistic_forppRef5TeV_v3-v3/MINIAODSIM'
config.Data.inputDataset = '/QCD_PtGT15_TuneCH3_5p02TeV_herwig7/RunIIpp5Spring18MiniAOD-94X_mc2017_realistic_forppRef5TeV-v2/MINIAODSIM'

config.Data.inputDBS = 'global'
config.Data.publication = False
config.Data.totalUnits = -1
config.Data.unitsPerJob = 10000
config.Data.splitting = 'EventAwareLumiBased'
config.JobType.maxMemoryMB = 3000
config.JobType.numCores = 1
config.JobType.maxJobRuntimeMin = 120
#config.Data.totalUnits = 2050
#config.Data.unitsPerJob = 1
#config.Data.splitting = 'Automatic'
#config.Data.splitting = 'FileBased'

config.Data.outLFNDirBase = '/store/group/phys_heavyions/aholterm/g2qqbar'
config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'
