from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsername
username = getUsername()

###############################################################################
# INPUT/OUTPUT SETTINGS

jobTag = 'Run3_OO_IonPhysics_runXXXXXX'
input = '/Hijing_MinimumBias_b015_OO_5362GeV/wangj-MINIAOD_250518_el8_Run3_2025_OXY_1506-2f28ed489918bb46492162f28bc838f0/USER'
#inputDatabase = 'global'
inputDatabase = 'phys03'
output = '/store/group/phys_heavyions/' + username + '/Run3_OO_2025Data_FastPrivateReco/'
outputServer = 'T2_CH_CERN'

###############################################################################

config = config()

config.General.requestName = jobTag
config.General.workArea = 'CrabWorkArea'
config.General.transferOutputs = True

config.JobType.psetName = 'forest_CMSSWConfig_Run3_OO_DATA_miniAOD.py'
config.JobType.pluginName = 'Analysis'
config.JobType.maxMemoryMB = 5000
config.JobType.pyCfgParams = ['noprint']
#config.JobType.scriptExe = 'runCrabWithEmap.sh'
#config.JobType.inputFiles = ['emap_2025_full.txt']
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
