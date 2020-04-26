#!/bin/sh
echo "CPU info"
sudo lshw -class processor | head -n 13
sleep 5
clear

echo "kernel version"
uname -r
sleep 5
clear

sudo dmesg --clear
stdinLoc="./OS_PJ1_Test/TIME_MEASUREMENT.txt"
echo "running "${stdinLoc}
echo "stdout:"
sudo ./bin/main < $stdinLoc
echo "dmesg:"
dmesg | grep Project1
sleep 7
clear

sudo dmesg --clear
stdinLoc="./OS_PJ1_Test/FIFO_1.txt"
echo "running "${stdinLoc}
echo "stdout:"
sudo ./bin/main < $stdinLoc
echo "dmesg:"
dmesg | grep Project1
sleep 7
clear

sudo dmesg --clear
stdinLoc="./OS_PJ1_Test/PSJF_2.txt"
echo "running "${stdinLoc}
echo "stdout:"
sudo ./bin/main < $stdinLoc
echo "dmesg:"
dmesg | grep Project1
sleep 7
clear

sudo dmesg --clear
stdinLoc="./OS_PJ1_Test/RR_3.txt"
echo "running "${stdinLoc}
echo "stdout:"
sudo ./bin/main < $stdinLoc
echo "dmesg:"
dmesg | grep Project1
sleep 7
clear

sudo dmesg --clear
stdinLoc="./OS_PJ1_Test/SJF_4.txt"
echo "running "${stdinLoc}
echo "stdout:"
sudo ./bin/main < $stdinLoc
echo "dmesg:"
dmesg | grep Project1
sleep 7
clear

sudo dmesg --clear
echo "end demo"