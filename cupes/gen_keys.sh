#!/bin/bash
exp="2022-10-30"
input="names.txt"
output="pubkeyes.txt"
dir="files"
instance="cupes"

#Clean up previous directories
rm -rf $dir
mkdir -p $dir

#Generate keys
while read -r line
do
	ssh-keygen -t rsa -f $dir/$line -C $line -b 2048 -q -P ""
	var=$line':'$(eval cat $dir/$line.pub)' google-ssh {"userName":"'$line'","expireOn":"'$exp'"}'
	echo $var
done < "$input"

#Get metadata
metadata=$(gcloud compute instances describe $instance)
