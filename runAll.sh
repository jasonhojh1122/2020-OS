#!/bin/sh

sudo dmesg --clear
stdinLoc="./OS_PJ1_Test/TIME_MEASUREMENT.txt"
stdoutLoc="./output/TIME_MEASUREMENT.txt"
dmesgoutLoc="./output/TIME_MEASUREMENT_dmesg.txt"

echo "running "${stdinLoc}
sudo ./bin/main < $stdinLoc > $stdoutLoc
dmesg | grep Project1 > $dmesgoutLoc

for i in $(seq 1 5)
do
    sudo dmesg --clear
    stdinLoc="./OS_PJ1_Test/FIFO_"${i}".txt"
    stdoutLoc="./output/FIFO_"${i}".txt"
    dmesgoutLoc="./output/FIFO_"${i}"_dmesg.txt"

    echo "running "${stdinLoc}
    sudo ./bin/main < $stdinLoc > $stdoutLoc
    dmesg | grep Project1 > $dmesgoutLoc
done

for i in $(seq 1 5)
do
    sudo dmesg --clear
    stdinLoc="./OS_PJ1_Test/RR_"${i}".txt"
    stdoutLoc="./output/RR_"${i}".txt"
    dmesgoutLoc="./output/RR_"${i}"_dmesg.txt"

    echo "running "${stdinLoc}
    sudo ./bin/main < $stdinLoc > $stdoutLoc
    dmesg | grep Project1 > $dmesgoutLoc
done

for i in $(seq 1 5)
do
    sudo dmesg --clear
    stdinLoc="./OS_PJ1_Test/SJF_"${i}".txt"
    stdoutLoc="./output/SJF_"${i}".txt"
    dmesgoutLoc="./output/SJF_"${i}"_dmesg.txt"

    echo "running "${stdinLoc}
    sudo ./bin/main < $stdinLoc > $stdoutLoc
    dmesg | grep Project1 > $dmesgoutLoc
done

for i in $(seq 1 5)
do
    sudo dmesg --clear
    stdinLoc="./OS_PJ1_Test/PSJF_"${i}".txt"
    stdoutLoc="./output/PSJF_"${i}".txt"
    dmesgoutLoc="./output/PSJF_"${i}"_dmesg.txt"

    echo "running "${stdinLoc}
    sudo ./bin/main < $stdinLoc > $stdoutLoc
    dmesg | grep Project1 > $dmesgoutLoc
done

