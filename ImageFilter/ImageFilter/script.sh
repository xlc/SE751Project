#!/bin/sh

# copy this script to executable folder before run it

processes=150
taskqueues="TheadPoolTaskQueue GCDTaskQueue SequentialTaskQueue"
imageFile=/Users/xiliangchen/1.jpg

for queue in $taskqueues
do
    echo "running on $queue..."
    rm $queue.txt > /dev/null 2>&1
    d=`date +%s`
    d+=2    # start after 2 seconds
    for i in $(seq 1 $processes)
    do
        ./ImageFilter -n YES -t $queue -i $imageFile -g 3 -f Grayscale -d $d >> $queue.txt &
    done
    wait
    echo "Time Taken"
    awk '{total+=$1;count+=1} END {print total/count; print count}' $queue.txt
done