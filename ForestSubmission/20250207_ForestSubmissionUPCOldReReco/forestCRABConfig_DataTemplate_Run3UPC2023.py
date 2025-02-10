from WMCore.Configuration import Configuration
from CRABClient.UserUtilities import getUsername

run = '$RUN'
jobTag = '$JOBTAG'
dataset = '$DATASET'
database = '$DATABASE'
username = getUsername()

config = Configuration()

config.section_("General")
config.General.requestName = jobTag
config.General.workArea = 'WorkArea'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'forestCMSSWConfig_miniAOD_Run3UPC.py'
config.JobType.maxMemoryMB = 2500
config.JobType.maxJobRuntimeMin = 120
config.JobType.scriptExe = 'ZDCemapScript.sh'
config.JobType.inputFiles = ['ZDCemap_PbPbUPC2023.txt']
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
config.Data.inputDataset = dataset
config.Data.inputDBS = database
config.Data.runRange = run
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 1000
config.Data.lumiMask = '/eos/user/c/cmsdqm/www/CAF/certification/Collisions23HI/Cert_Collisions2023HI_374288_375823_Golden.json'
#config.Data.outLFNDirBase = '/store/user/' + username + '/run3_2023Data_Dec2024ReReco/' + config.General.requestName
config.Data.outLFNDirBase = '/store/group/phys_heavyions/' + username + '/run3_2023Data_Jan2024ReReco/Run3_2023UPC_' + run
config.Data.publication = False
config.Data.allowNonValidInputDataset = True

config.section_("Site")
#config.Site.whitelist = ['T2_US_Vanderbilt', 'T2_US_Nebraska', 'T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'
