#!/bin/sh

# copy this script to executable folder before run it

taskqueues="GCDTaskQueue SequentialTaskQueue"

sleeptime=1
taskcount=128
for queue in $taskqueues
do
	outfile=idletask-$queue-$tps.txt
	echo "start for $outfile" >&2
	rm $outfile > /dev/null 2>&1
	for task in $(seq 1 $taskcount)
	do
		echo ./ImageFilter-debug -n YES -t $queue -tp $tps -nc YES -st $sleeptime -ts $task 
		./ImageFilter-debug -n YES -t $queue -tp $tps -nc YES -st $sleeptime -ts $task >> $outfile
	done
done
