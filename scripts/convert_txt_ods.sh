#!/bin/bash

while read dirs
do
	ls $dirs | grep txt > files.txt
	echo "Proccessing directory $dirs" 
	i=0;
	while read files
	do
	  soffice --convert-to ods --outdir $dirs $dirs/$files --calc
	done < "./files.txt"
done < "./dirs.txt"