#!/bin/bash

IFS='
'

ls | grep -v '\.' > dirs.txt

while read dirs
do
	a=$(echo $dirs | awk {'print $1'} )
	b=$(echo $dirs | awk {'print $2'} )
	c=$(echo $dirs | awk {'print $3'} )
	
	mv $(echo $dirs) $(echo $a)_$(echo $b)_$(echo $c)

	ls $(echo $a)_$(echo $b)_$(echo $c) | grep txt > files.txt
	while read files
	do
	  fa=$(echo $files | awk {'print $1'} )
	  fb=$(echo $files | awk {'print $2'} )
	  fc=$(echo $files | awk {'print $3'} )
	  
	  mv $(echo $a)_$(echo $b)_$(echo $c)/$(echo $files) $(echo $a)_$(echo $b)_$(echo $c)/$(echo $fa)_$(echo $fb)_$(echo $fc)
	done < "./files.txt"
done < "./dirs.txt"