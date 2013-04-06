#!/bin/bash

make

echo "Merge Sort"
for i in 1 2 3 4 5 6 7 8 9 10
do
	for n in 100 1000 10000 100000 1000000 10000000
	do
		echo "----------------------"
		echo "Number of threads = " $i
		echo "Problem Size = " $n
		./mergeSort $i $n
		echo
	done
done


echo "Unique String"
for i in 1 2 3 4 5 6 7 8 9 10
do
	echo "------------------------"
	echo "Number of threads = " $i
	./uniqueString $i < testfile
	echo 
done
