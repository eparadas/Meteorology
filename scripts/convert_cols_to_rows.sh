#!/bin/bash

echo "removing old linearized files..."
rm -f linearized_*.txt 
rm -f read_files.txt

ls | grep -v '\.' > dirs.txt

touch read_files.txt
delim=$3

while read dirs
do
  echo "Proccessing directory $dirs"
  ls $dirs | grep txt > files.txt
  while read files
  do
  	file_is_read=false
  	while read r_fs
  	do
  		if [ $files == $r_fs ]
  		then
  				file_is_read=true
  				break
  		fi
  	done < "./read_files.txt"
  	if ! $file_is_read
  	then
  		dd=$(echo $files | cut -d '/' -f2 | cut -d "$delim" -f1)
  		mm=$(echo $files | cut -d '/' -f2 | cut -d "$delim" -f2)
  		yy=$(echo $files | cut -d '/' -f2 | cut -d "$delim" -f3 | cut -d '.' -f1)
  		
			/mnt/storage_local/pkassom/scripts/Cols_to_Rows.exe $dirs/$files $dd $mm $yy $1 $2
			echo $files >> read_files.txt
	  fi
  done < "./files.txt"
  
done < "./dirs.txt"
