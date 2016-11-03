#!/bin/bash

# unoconv -l &

while read dirs
do
	ls $dirs | grep txt > files.txt
	echo "Proccessing directory $dirs" 
	i=0;
	while read files
	do
	   ods_file=$(basename $files txt)
	   unoconv -v -f xls -o $dirs $dirs/$(echo $ods_file)ods
	done < "./files.txt"
done < "./dirs.txt"