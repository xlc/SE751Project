#!/bin/sh

# copy this script to executable folder before run it

processes=30
taskqueues="TheadPoolTaskQueue GCDTaskQueue SequentialTaskQueue"
imageFile=/Users/xiliangchen/1.jpg

for queue in $taskqueues
do
    echo "running on $queue..."
    rm result.txt > /dev/null 2>&1
    for i in $(seq 1 $processes)
    do
        ./ImageFilter -n YES -t $queue -i $imageFile -g 2 -f Grayscale >> result.txt &
    done
    wait
    echo "Time Taken"
    awk '{total+=$1;count+=1} END {print total/count}' result.txt
done