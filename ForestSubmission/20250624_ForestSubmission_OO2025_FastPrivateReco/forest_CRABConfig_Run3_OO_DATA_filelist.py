from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsername
username = getUsername()

###############################################################################
# INPUT/OUTPUT SETTINGS

jobTag = 'Run3_OO_IonPhysics_runXXXXXX'
inputFilelist = '/path/to/filelist.txt'

output = '/store/group/phys_heavyions/' + username + '/Run3_OO_2025Data_FastPrivateReco/'
outputServer = 'T2_CH_CERN'

Data.userInputFiles = open('/path/to/local/file.txt').readlines()

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

config.Data.outputPrimaryDataset = jobTag
config.Data.userInputFiles = open(inputFilelist).readlines()
config.Data.outLFNDirBase = output
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = -1
config.Data.publication = False
config.Data.allowNonValidInputDataset = True

#config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = outputServer
