#!/bin/bash

# DO THE MC
nohup ./RunParallelMC_xrdcp.sh 0 > logs/process_mc_0.log 2>&1 &
wait
nohup ./RunParallelMC_xrdcp.sh 1 > logs/process_mc_1.log 2>&1 &
wait
nohup ./RunParallelMC_xrdcp.sh 2 > logs/process_mc_2.log 2>&1 &
wait
nohup ./RunParallelMC_xrdcp.sh 3 > logs/process_mc_3.log 2>&1 &
wait
nohup ./RunParallelMC_xrdcp.sh 4 > logs/process_mc_4.log 2>&1 &
wait
nohup ./RunParallelMC_xrdcp.sh 5 > logs/process_mc_5.log 2>&1 &
wait 
nohup ./RunParallelMC_xrdcp.sh 6 > logs/process_mc_6.log 2>&1 &
wait

echo "done with MC"

# DATA
#nohup ./RunParallelData_xrdcp.sh 0 A > logs/process_data_0a.log 2>&1 &
#wait
#nohup ./RunParallelData_xrdcp.sh 1 A > logs/process_data_1a.log 2>&1 &
#wait
#nohup ./RunParallelData_xrdcp.sh 2 A > logs/process_data_2a.log 2>&1 &
#wait
nohup ./RunParallelData_xrdcp.sh 3 A > logs/process_data_3a.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 4 A > logs/process_data_4a.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 5 A > logs/process_data_5a.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 6 A > logs/process_data_6a.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 7 A > logs/process_data_7a.log 2>&1 &
wait
nohup ./RunParallelData_xrdcp.sh 8 A > logs/process_data_8a.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 9 A > logs/process_data_9a.log 2>&1 &
wait
echo "done with DATA A"

nohup ./RunParallelData_xrdcp.sh 0 B > logs/process_data_0b.log 2>&1 &
wait
nohup ./RunParallelData_xrdcp.sh 1 B > logs/process_data_1b.log 2>&1 &
wait
nohup ./RunParallelData_xrdcp.sh 2 B > logs/process_data_2b.log 2>&1 &
wait
nohup ./RunParallelData_xrdcp.sh 3 B > logs/process_data_3b.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 4 B > logs/process_data_4b.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 5 B > logs/process_data_5b.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 6 B > logs/process_data_6b.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 7 B > logs/process_data_7b.log 2>&1 &
wait
echo "done with DATA B"

nohup ./RunParallelData_xrdcp.sh 0 C > logs/process_data_0c.log 2>&1 &
wait
nohup ./RunParallelData_xrdcp.sh 1 C > logs/process_data_1c.log 2>&1 &
wait
nohup ./RunParallelData_xrdcp.sh 2 C > logs/process_data_2c.log 2>&1 &
wait
nohup ./RunParallelData_xrdcp.sh 3 C > logs/process_data_3c.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 4 C > logs/process_data_4c.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 5 C > logs/process_data_5c.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 6 C > logs/process_data_6c.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 7 C > logs/process_data_7c.log 2>&1 &
wait
nohup ./RunParallelData_xrdcp.sh 8 C > logs/process_data_8c.log 2>&1 &
wait 
nohup ./RunParallelData_xrdcp.sh 9 C > logs/process_data_9c.log 2>&1 &
wait
echo "done with DATA C"