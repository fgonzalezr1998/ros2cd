# ros2cd

ROS2 version of *roscd* command.

You can use this tool to go to the path where the source code of a ROS2 package is found.

## Usage Instructions

### Compile de Source Code:

```
$ gcc -c -Wall -Wshadow ros2ppr.c
$ gcc -o ros2ppr ros2ppr.o
```

These commands will have created an executable named *ros2prp* (ros2 Package Path Resolution).

### Move the executable to $PATH

Now, you have to put the executable created previously in one of the paths of the $PATH environment variable. For example: */usr/local/bin/*.

Also, you can add a new path to the $PATH.

### Source ros2cd.bash

Finally, you have to type ``source ros2cd.bash``.

I recommend to move this file to your home, for example and add the next line to your *.bashrc* file:

```
source ros2cd.bash
```

Once time you have made these steps, you need to *configure* this tool

### Configuration

This tool is prepared for reading its configuration from the next path: ``$HOME/.ros2cd/paths.txt``.

So, you need to create this file. The content of this file must be all paths where you want that *ros2cd* look for the requested packages (one path per line). This is an example:

```
/home/user/ros2_ws/src
/home/user/second_ws/src
/opt/ros/foxy/share
```

**That is all you need for using this tool!**

Example:

```
$ ros2cd rviz2
```
