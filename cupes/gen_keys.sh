#!/bin/bash
exp="2022-10-30"
input="names.txt"
output="pubkeys.txt"
sudo="sudo.txt"
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

#Generate sudo remove file
rm -rf $sudo
echo "#!/bin/bash" >>$sudo
while read -r line
do
	echo "sudo deluser $line google-sudoers" >>$sudo
done < "$input"

#Get metadata
#metadata=$(gcloud compute instances describe $instance) --format("metadata")

#Add ssh keys meta data from describe command to $output
#Set metadata with additional keys
#gcloud compute instances add-metadata VM_NAME --metadata-from-file ssh-keys=$output


#Mail the private ssh keys from this computer
#python 0365mail.py


#Update startup-script in VMs custom-metadata with the contents of $sudo and restart the VM.
