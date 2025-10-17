# Skimmer for Low-pT Dzero

## Structure

`RunParallelData.sh` and `RunParallelMC.sh` call `ProcessLocalSkim.sh` or
`ProcessXRDSkim.sh` depending on their settings.

## Instructions

### 1) Modify "RunParallel" script

### 2) Clean
```bash
source clean.sh
```

### 3) Authenticate
For accessing files through xrootd:
```bash
voms-proxy-init -rfc -voms cms
```

For accessing files on eos:
```bash
kinit -5
```

### 4) Run "RunParallel" script
```bash
# For data:
bash RunParallelData.sh

# For MC:
bash RunParallelMC.sh
```

> [!TIP]
> If you know your skim will take a long time **and if you have confirmed that
> it works without errors** you can use `nohup` to run it in the background 
> and persist after logging out:
> ```bash
> nohup bash RunParallelData.sh > log.txt &
> ```

### 5) Monitor
Regularly check on the status of the skimmer to make sure it is working
as expected.

If you are using `nohup`, you can check the status with:
```bash
vim -R log.txt
```
This opens the log file in "read-only" mode so the skimmer can still 
write to it.
