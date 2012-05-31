#!/bin/sh

# copy this script to executable folder before run it

filters="Edge Grayscale"
taskqueues="GCDTaskQueue SequentialTaskQueue"
images="2.jpg"

processes=100
# variable processes count, fix tasks count
# 1 to 100 processes, 128 tasks
for imageFile in $images
do
    for filter in $filters
    do
        for queue in $taskqueues
        do
            outfile=variable_processes-$processes-$imageFile-$filter-$queue.txt
            echo "start for $outfile" >&2
            rm $outfile > /dev/null 2>&1
			for processcount in $(seq 1 5 $processes)
			do
				rm $outfile.tmp > /dev/null 2>&1
				echo $processcount
				for i in {1..3}
				do
					rm result.txt > /dev/null 2>&1
					d=$(date +%s)
					d=$(($d+2))    # start after 2 seconds
					for i in $(seq 1 $processcount)
					do
						./ImageFilter -n YES -t $queue -i $imageFile -g 3 -f $filter -d $d >> result.txt &
					done
					wait
					awk '{total+=$1;count+=1} END {print total/count}' result.txt >> $outfile.tmp
				done
				awk '{total+=$1;count+=1} END {print total/count}' $outfile.tmp >> $outfile
            done
        done
    done
done

rm *.tmp > /dev/null 2>&1