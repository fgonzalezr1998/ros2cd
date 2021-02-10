#! /bin/bash

ros2cd() {
	output=$(./ros2cd/ros2cd $1)
	if [ $? -eq 0 ]
	then
		cd $output
	else
		echo $output
	fi
}
