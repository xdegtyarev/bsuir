#!/bin/bash

#TASK

#1.2
#search for file by name,
#search is set by list as argument
#search from current dir

#2.5
#for files with matched header(first string)
#cat its content by strings with string enumeration
#cat -n
#header is fixed

#3.1
#to stderr
#start directory [pwd]
#username [$USER]
#currentdata [$(date -u)]
#time of scenario [$[end-start]]

#END
echo "hello, it is IPR1"
start=$(date +%s)
searchwords=${@:2} #skipping first one to search for catalog
ldir=$1 #setting basedir
shopt -s extglob nullglob

traverse () {
	cd "$1"
	#1.2
	for d in *; do #go over all files in current dir
		if [[ -d "$1"/"$d" ]]; then #check if d is directory
			traverse "$1"/"$d" $(($2 + 1)) #go deeper "$1"/"$d"
		elif [[ -e "$1"/"$d" ]]; then #check if d is file and matches any doc in header
			#2.5
			for searchword in $searchwords; do
				if [[ $d =~ $searchword ]]; then
					echo "Found file: '"$d"' that matched searchword '"$searchword"'"
					cat -b "$1"/"$d"
					echo "EOF"
				fi
			done
		else
			echo "No directory or file occurred"
		fi
	done
}

traverse $ldir 0
duration=$(echo "$(date +%s) - $start" | bc)
#3.1
echo "DIR:" $ldir "USR:" $USER "DATE:" $(date -u) "DUR:" $duration " sec" >&2