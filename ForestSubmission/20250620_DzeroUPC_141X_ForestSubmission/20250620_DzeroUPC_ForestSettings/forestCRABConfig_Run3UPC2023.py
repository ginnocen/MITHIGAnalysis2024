from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsername

pd = '10'
jobTag = 'Run3UPC_2023Data_Jan2024ReReco_HIForward' + pd
dataset = '/HIForward' + pd + '/HIRun2023A-14Feb2025-v1/MINIAOD'
username = getUsername()

config = config()

config.General.requestName = jobTag
config.General.workArea = 'CrabWorkArea'
config.General.transferOutputs = True

config.JobType.psetName = 'forestCMSSWConfig_miniAOD_Run3UPC_2023Data_Feb2025ReReco.py'
config.JobType.pluginName = 'Analysis'
config.JobType.maxMemoryMB = 5000
config.JobType.pyCfgParams = ['noprint']
config.JobType.allowUndistributedCMSSW = True

config.Data.outLFNDirBase = '/store/user/' + username + '/public/2025_June/' + jobTag
config.Data.inputDataset = dataset
config.Data.unitsPerJob = 50000
config.Data.totalUnits = -1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.lumiMask = '/eos/cms/store/group/phys_heavyions/jdlang/Run3UPC/Cert_Collisions2023HI_374288_375823_Good_ZDC_Golden.json'
config.Data.publication = False

config.Site.storageSite = 'T2_US_MIT'
