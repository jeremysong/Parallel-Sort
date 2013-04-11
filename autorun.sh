#!/bin/bash

make

echo "Merge Sort"
for i in 1 2 4 8 16 32
do
	for n in 100 1000 10000 100000 1000000
	do
		echo "----------------------"
		echo "Number of threads = " $i
		echo "Problem Size = " $n
		./mergeSort $i $n
		echo
	done
done


echo "Unique String"
for i in 1 2 4 8 16 32
do
	echo "------------------------"
	echo "Number of threads = " $i
	./uniqueString $i < testfile
	echo 
done
