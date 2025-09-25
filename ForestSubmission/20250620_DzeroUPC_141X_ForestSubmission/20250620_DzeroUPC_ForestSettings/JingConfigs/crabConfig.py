#

from CRABClient.UserUtilities import config
config = config()
config.General.transferOutputs = True
# ==============================
config.General.requestName = 'HiForest_250522_HIForward0_HIRun2023A_16Jan2024'
# config.Data.inputDataset = '/HIForward0/HIRun2023A-16Jan2024-v1/MINIAOD'
config.Data.inputDataset = '/HIForward0/HIRun2023A-16Jan2024-v1/MINIAOD'
# config.Data.runRange = '374810-374810'
config.Data.lumiMask = '/eos/user/c/cmsdqm/www/CAF/certification/Collisions23HI/Cert_Collisions2023HI_374288_375823_Good_ZDC_Golden.json'
# ==============================
config.General.workArea = 'crab_projects'
config.JobType.psetName = 'forest_miniAOD_run3_UPC_DATA_wDfinder_run2023.py'

# config.JobType.pluginName = 'PrivateMC'
config.JobType.pluginName = 'Analysis'
config.JobType.maxMemoryMB = 5000
config.JobType.pyCfgParams = ['noprint']
# config.JobType.numCores = 4
# config.JobType.scriptExe = 'submitScript.sh'
# config.JobType.inputFiles = ['CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run3v1302x04_offline_374289.db']
config.JobType.allowUndistributedCMSSW = True

##
# config.Data.inputDBS = 'phys03'
config.Data.unitsPerJob = 50000 ##
config.Data.totalUnits = -1
# config.Data.splitting = 'FileBased'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
# config.Data.outputDatasetTag = 'reMINIAOD_230929_calojet5_373710'
# config.Data.outLFNDirBase = '/store/group/phys_heavyions/wangj/Forest2023'

##
# config.Site.storageSite = 'T2_CH_CERN'
config.Site.storageSite = 'T2_US_MIT'
# config.Site.whitelist = ['T2_US_MIT']
# config.Site.blacklist = ['T2_US_Nebraska','T2_US_UCSD','T2_US_Wisconsin','T2_FR_GRIF_IRFU','T3_US_Rutgers','T3_BG_UNI_SOFIA','T3_IT_Perugia']
# config.Site.ignoreGlobalBlacklist = True
# config.section_("Debug")
# config.Debug.extraJDL = ['+CMS_ALLOW_OVERFLOW=False']
