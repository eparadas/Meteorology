#!/bin/bash

while read dirs
do
	ls $dirs | grep txt > files.txt
	echo "Proccessing directory $dirs" 
	i=0;
	while read files
	do
		sed -i -e 's/\,/\./g' $(echo $dirs)/$(echo $files)
		i=$(( i + 1 ))
  done < "./files.txt"
  echo "$i files converted"
  echo ""
done < "./dirs.txt" 
	