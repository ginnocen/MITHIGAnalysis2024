from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsername
username = getUsername()

###############################################################################
# INPUT/OUTPUT SETTINGS

jobTag = 'Run3_OO_IonPhysics_runXXXXXX'
input = 'DAS_PATH'
inputDatabase = 'global'
output = '/store/group/phys_heavyions/' + username + '/Run3_OO_2025Data_FastPrivateReco/'
outputServer = 'T2_CH_CERN'

###############################################################################

config = Configuration()

config.General.requestName = jobTag
config.General.workArea = 'CrabWorkArea'
config.General.transferOutputs = True

config.JobType.psetName = 'forest_CMSSWConfig_Run3_OO_DATA_miniAOD.py'
config.JobType.pluginName = 'Analysis'
config.JobType.maxMemoryMB = 5000
config.JobType.pyCfgParams = ['noprint']
#config.JobType.scriptExe = 'ZDCemapScript.sh'
#config.JobType.inputFiles = ['ZDCemap_PbPbUPC2023.txt']
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = input
config.Data.inputDBS = inputDatabase
config.Data.outLFNDirBase = output
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 10000
config.Data.totalUnits = -1
config.Data.publication = False
config.Data.allowNonValidInputDataset = True

config.Site.storageSite = outputServer
