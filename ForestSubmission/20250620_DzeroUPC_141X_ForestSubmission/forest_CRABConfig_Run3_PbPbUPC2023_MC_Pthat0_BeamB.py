from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsername
username = getUsername()

###############################################################################
# INPUT/OUTPUT SETTINGS

jobTag = 'Run3_PbPbUPC2023_gammaNToD0_Pthat0_BeamB'
input = '/GNucleusToD0-PhotonBeamB_Pthat0_UPC_5p36TeV_pythia8_evtgen/yuchenc-RECO_v2-297e19fe0ed230e371d18bc15610b14e/USER'
inputDatabase = 'phys03'
output = '/store/group/phys_heavyions/' + username + '/Run3_PbPbUPC/Forest_2023MC/'
outputServer = 'T2_CH_CERN'

###############################################################################

config = config()

config.General.requestName = jobTag
config.General.workArea = 'CrabWorkArea'
config.General.transferOutputs = True

config.JobType.psetName = 'forest_CMSSWConfig_Run3_PbPbUPC2023_MC_Dfinder.py'
config.JobType.pluginName = 'Analysis'
config.JobType.maxMemoryMB = 5000
config.JobType.pyCfgParams = ['noprint']
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = input
config.Data.inputDBS = inputDatabase
config.Data.unitsPerJob = 50000
config.Data.totalUnits = -1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Data.outLFNDirBase = output
config.Site.storageSite = outputServer
