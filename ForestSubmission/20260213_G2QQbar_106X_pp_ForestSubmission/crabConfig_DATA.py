#file path will be:
#outLFNDirBase/inputDataset/requestName/time_tag/...
from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'g2bbbars_DATA_HFfindersC'

config.section_('JobType')
config.JobType.psetName = 'recipe5_DATA.py'
config.JobType.pluginName = 'Analysis'
config.JobType.allowUndistributedCMSSW = True


config.section_("Data")
#config.Data.inputDataset = '/QCD_pThat-15_bJet_TuneCP5_5p02TeV-pythia8/RunIISummer20UL17pp5TeVMiniAODv2-106X_mc2017_realistic_forppRef5TeV_v3-v3/MINIAODSIM'
#config.Data.inputDataset = '/QCD_pThat-15_Dijet_TuneCP5_5p02TeV-pythia8/RunIISummer20UL17pp5TeVMiniAODv2-106X_mc2017_realistic_forppRef5TeV_v3-v3/MINIAODSIM'
config.Data.inputDataset = '/LowEGJet/Run2017G-UL2017_MiniAODv2-v2/MINIAOD'

#config.Data.lumiMask = 'Cert_306546-306826_5TeV_EOY2017ReReco_Collisions17_JSON.txt'
config.Data.lumiMask = 'collisionsC.json'
config.Data.inputDBS = 'global'
config.Data.publication = False
config.Data.totalUnits = -1
#config.Data.unitsPerJob = 28000
config.Data.unitsPerJob = 1
config.Data.splitting = 'LumiBased'
config.JobType.maxMemoryMB = 3000
config.JobType.maxJobRuntimeMin = 180
#config.Data.totalUnits = 2050
#config.Data.unitsPerJob = 1
#config.Data.splitting = 'Automatic'
#config.Data.splitting = 'FileBased'

config.Data.outLFNDirBase = '/store/group/phys_heavyions/aholterm/g2qqbar'
config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'
