# stopwatch
Command line stopwatch written in C with split / lap support
##Usage :##
Run stopwatch :
```
[root@alfie trunk]# stopwatch
Time Elapsed : 2.700s
```
Press spacebar to get lap times :
```
[root@alfie trunk]# stopwatch
Lap Time : 3.686, Split Time : 3.686s
Lap Time : 4.968, Split Time : 8.654s
Time Elapsed : 11.000s
```
Press Enter to stop the stopwatch :
```
[root@alfie trunk]# stopwatch
Lap Time : 3.686, Split Time : 3.686s
Lap Time : 4.968, Split Time : 8.654s
Total Time Elapsed : 12.502s
```
What command line arguments does it take? :
```
[root@alfie trunk]# stopwatch -h
Usage : stopwatch [ -vh ] [ -p interval ]
Where : -p interval : The interval in ms to print out elapsed time.
        -v          : Print out version information.
        -h          : Print out this message.
```
##Installation Instructions :##
Run `make install` as root
##Uninstallation Instructions :##
Run `make uninstall` as root
