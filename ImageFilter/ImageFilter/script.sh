#!/bin/sh

# copy this script to executable folder before run it

filters="Grayscale Blur"
taskqueues="TheadPoolTaskQueue GCDTaskQueue SequentialTaskQueue"
images="1.jpg 2.jpg"

tasks=20
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
            for count in $(seq 1 $tasks)
            do
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

processes=200
# variable processes count, fix tasks count
# 1 to 200 processes, 128 tasks
for imageFile in $images
do
    for filter in $filters
    do
        for queue in $taskqueues
        do
            outfile=variable_processes-$processes-$imageFile-$filter-$queue.txt
            echo "start for $outfile" >&2
            rm $outfile > /dev/null 2>&1
			for processcount in $(seq 1 $processes)
			do
				rm result.txt > /dev/null 2>&1
				d=$(date +%s)
				d=$(($d+2))    # start after 2 seconds
				for i in $(seq 1 $processcount)
				do
					./ImageFilter -n YES -t $queue -i $imageFile -g 5 -f $filter -d $d >> result.txt &
				done
				wait
				awk '{total+=$1;count+=1} END {print total/count}' result.txt >> $outfile
            done
        done
    done
done