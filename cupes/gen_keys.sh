#!/bin/bash
exp="2022-10-30"
input="names.txt"
output="pubkeys.txt"
dir="files"

zone=:wq

instance="cupes"

#Clean up previous directories
rm -f $output
rm -rf $dir
mkdir -p $dir

#Generate keys
while read -r line
do
	line=$(echo $line | cut -f1 -d\@)
	ssh-keygen -t rsa -f $dir/$line -C $line -b 2048 -q -P ""
	var=$line':'$(eval cat $dir/$line.pub)' google-ssh {"userName":"'$line'","expireOn":"'$exp'"}'
	echo $var >>$output
done < "$input"

#Get metadata
#metadata=$(gcloud compute instances describe $instance) --format("metadata")

#Add ssh keys meta data from describe command to $output

#Set metadata with additional keys
#gcloud compute instances add-metadata VM_NAME --metadata-from-file ssh-keys=$output


#Mail the private ssh keys
#python 0365mail.py

