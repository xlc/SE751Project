#!/bin/sh

# copy this script to executable folder before run it

filters="Grayscale Blur "
taskqueues="GCDTaskQueue SequentialTaskQueue"
images="2.jpg"

tasks=18
# fix processes count, variable tasks count
# 1 process, 1 to 2^20 tasks
for imageFile in $images
do
    for filter in $filters
    do
        for queue in $taskqueues
        do
            outfile=variable_tasks-$tasks-$imageFile-$filter-$queue.txt
            echo "start for $outfile" >&2
            rm $outfile > /dev/null 2>&1
            for count in $(seq 2 $tasks)
            do
            	echo $count
				rm result.txt > /dev/null 2>&1
				# run 3 times and use average time
				./ImageFilter -n YES -t $queue -i $imageFile -ts $count -f $filter >> result.txt
				./ImageFilter -n YES -t $queue -i $imageFile -ts $count -f $filter >> result.txt
				./ImageFilter -n YES -t $queue -i $imageFile -ts $count -f $filter >> result.txt
				awk '{total+=$1;count+=1} END {print total/count}' result.txt >> $outfile
            done
        done
    done
done