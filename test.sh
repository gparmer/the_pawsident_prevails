#!/bin/sh
TIME=/usr/bin/time

echo "Forkjoin:"
$TIME -f "%E" -o times.txt ./bin 100000 1 fj
echo "Execution time" `cat times.txt`
$TIME -f "%E" -o times.txt ./bin 100000 2 fj
echo "Execution time" `cat times.txt`
$TIME -f "%E" -o times.txt ./bin 100000 4 fj
echo "Execution time" `cat times.txt`
$TIME -f "%E" -o times.txt ./bin 100000 8 fj
echo "Execution time" `cat times.txt`
$TIME -f "%E" -o times.txt ./bin 100000 16 fj
echo "Execution time" `cat times.txt`
rm times.txt

echo "Mapreduce:"
$TIME -f "%E" -o times.txt ./bin 100000 1 mr
echo "Execution time" `cat times.txt`
$TIME -f "%E" -o times.txt ./bin 100000 2 mr
echo "Execution time" `cat times.txt`
$TIME -f "%E" -o times.txt ./bin 100000 4 mr
echo "Execution time" `cat times.txt`
$TIME -f "%E" -o times.txt ./bin 100000 8 mr
echo "Execution time" `cat times.txt`
$TIME -f "%E" -o times.txt ./bin 100000 16 mr
echo "Execution time" `cat times.txt`
rm times.txt
