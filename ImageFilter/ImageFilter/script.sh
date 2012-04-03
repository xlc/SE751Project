#!/bin/sh

# copy this script to executable folder before run it

echo "running on TheadPoolTaskQueue..."
rm result.txt > /dev/null 2>&1
for i in {1..64}
do
	./ImageFilter -n YES -t ThreadPoolTaskQueue -i /Users/xiliangchen/1.jpg -g 2 -f Grayscale >> result.txt &
done
wait
echo "Time Taken"
awk '{total+=$1;count+=1} END {print total/count}' result.txt

echo "running on GCDTaskQueue..."
rm result.txt > /dev/null 2>&1
for i in {1..64}
do
	./ImageFilter -n YES -t GCDTaskQueue -i /Users/xiliangchen/1.jpg -g 2 -f Grayscale >> result.txt &
done
wait
echo "Time Taken"
awk '{total+=$1;count+=1} END {print total/count}' result.txt