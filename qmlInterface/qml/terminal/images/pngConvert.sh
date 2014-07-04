#!/bin/bash
find . -name "*.png" -print0 | \
while read -d $'\0' file
do
   temporaryFile="$file.asdf.tmp"
   convert "$file" -strip "$file" &> $temporaryFile
   if [[ -s $temporaryFile ]] ; then
     echo "Changed with reason: “$file”"
   else
     echo "Changed without reason, reverting: “$file”"
     git checkout -- "$file"
   fi
   rm "$temporaryFile"
done
