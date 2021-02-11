# ros2cd

ROS2 version of *roscd* command.

You can use this tool to go to the path where the source code of a ROS2 package is found.

## Usage Instructions

### Compile de Source Code:

```
$ gcc -c -Wall -Wshadow ros2ppr.c
$ gcc -o ros2ppr ros2ppr.o
```

These commands will have created an executable named *ros2prp* (ros2 Package Resolution Path).

### Move the executable to $PATH

Now, you have to put the executable created previously in one of the paths of the $PATH environment variable. For example: */usr/local/bin/*.
