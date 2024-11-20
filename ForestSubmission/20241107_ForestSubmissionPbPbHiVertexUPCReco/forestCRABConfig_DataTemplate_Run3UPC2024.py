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
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
config.Data.inputDataset = dataset
config.Data.inputDBS = database
config.Data.runRange = run
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 1000
config.Data.lumiMask = '/eos/cms/store/group/phys_heavyions/sayan/HIN_run3_pseudo_JSON/HIPhysicsRawPrime_2024/Golden_387853_continue_L1DeadTimeCut10percent.txt'
config.Data.outLFNDirBase = '/store/user/' + username + '/run3_2024PromptReco/' + config.General.requestName
config.Data.publication = False

config.section_("Site")
#config.Site.whitelist = ['T2_US_Vanderbilt', 'T2_US_Nebraska', 'T2_CH_CERN']
config.Site.storageSite = 'T2_US_MIT'
