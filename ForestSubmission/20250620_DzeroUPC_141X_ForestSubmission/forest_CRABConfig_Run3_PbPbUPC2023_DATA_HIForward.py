from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsername
username = getUsername()

###############################################################################
# INPUT/OUTPUT SETTINGS

pd = '0'
jobTag = 'Run3_PbPbUPC2023_HIForward' + pd
input = '/HIForward' + pd + '/HIRun2023A-14Feb2025-v1/MINIAOD'
inputDatabase = 'global'
output = '/store/group/phys_heavyions/' + username + '/Run3_PbPbUPC/Forest_2023_Feb2025ReReco/'
outputServer = 'T2_CH_CERN'

###############################################################################

config = config()

config.General.requestName = jobTag
config.General.workArea = 'CrabWorkArea'
config.General.transferOutputs = True

config.JobType.psetName = 'forest_CMSSWConfig_Run3_PbPbUPC2023_DATA_Dfinder.py'
config.JobType.pluginName = 'Analysis'
config.JobType.maxMemoryMB = 5000
config.JobType.pyCfgParams = ['noprint']
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = input
config.Data.inputDBS = inputDatabase
config.Data.outLFNDirBase = output
config.Data.splitting = 'EventAwareLumiBased'
config.Data.lumiMask = '/eos/user/c/cmsdqm/www/CAF/certification/Collisions23HI/Cert_Collisions2023HI_374288_375823_Good_ZDC_Golden.json'
config.Data.unitsPerJob = 50000
config.Data.totalUnits = -1
config.Data.publication = False
config.Data.allowNonValidInputDataset = False

config.Site.storageSite = outputServer
