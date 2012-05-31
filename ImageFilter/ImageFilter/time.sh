#!/bin/sh

# copy this script to executable folder before run it

filters="Grayscale Colorful Invert Sepia Edge Blur "
queue="ThreadPoolTaskQueue"
images="2.jpg 3.jpg"

threadpoolsize="4 16 64"

for tps in $threadpoolsize
do
	outfile=time-$queue-$tps.txt
	echo "start for $outfile" >&2
	rm $outfile > /dev/null 2>&1
	for filter in $filters
	do
		for imageFile in $images
		do
			sleep 0.5
			rm result.txt > /dev/null 2>&1
			# run 3 times and use average time
			echo ./ImageFilter -n YES -t $queue -i $imageFile -g 5 -f $filter -tp $tps
			./ImageFilter -n YES -t $queue -i $imageFile -g 5 -f $filter -tp $tps >> result.txt
			./ImageFilter -n YES -t $queue -i $imageFile -g 5 -f $filter -tp $tps >> result.txt
			./ImageFilter -n YES -t $queue -i $imageFile -g 5 -f $filter -tp $tps >> result.txt
			awk '{total+=$1;count+=1} END {print total/count}' result.txt >> $outfile
		done
	done
done