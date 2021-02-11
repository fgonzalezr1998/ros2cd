#! /bin/bash

ros2cd() {
	output=$(ros2prp $1)
	if [ $? -eq 0 ]
	then
		cd $output
	else
		echo $output
	fi
}
