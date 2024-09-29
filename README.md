
## Welcome!

This project is a simple tool that displays the CPU and memory usage of a system, as well as other information.

## Running the files

1. To run the files, you will need to compile the code in your compiler of choice:
    ex. gcc mySystemStats.c
2. Then, you will need to run the executable file:
    ./a.out

    The program also takes optional command arguments:

   --system: displays the system stats (memory and CPU data) only

   --user: displays the user and session stats only

   --graphics: adds a graphic display of the memory and CPU usage

   --sequential: runs the program in sequential mode where the program will run interactions, perfect to output to another file

   --samples=N: sets the number of samples to N (default is 10) 
        can also be activated by writing only a number after the program name, e.g. ./a.out 5

   --tdelay=T: sets the delay between samples to T seconds (default is 1) 
        can also be activated by writing only a number after the program name, but must also include a sample value e.g. ./a.out 5 2

4. The program will then run and display the system stats.

## Table of contents 
- [Overview](#overview)
  - [Built with](#built-with)
  - [Function insights](#function-insights)
  - [Continued development](#continued-development)
- [Author](#author) 

## Overview

### Built with
I built this with C on Linux, using the following libraries:
- stdio.h
- stdlib.h
- string.h
- sys/resource.h
- sys/utsname.h
- sys/sysinfo.h
- sys/types.h
- utmp.h
- unistd.h
- ctype.h 
- math.h

### Function Insight
main() - The main function of the program. It takes in command line arguments and runs the program accordingly. It also calls the other functions to display the system stats. It stores integers that represent if the command arguments have been activated and then runs the program accordingly. It also stores the number of samples and the time delay between samples. It then runs the program in a loop depending on the number of samples + 1, calling the other functions to display the system stats. Every time the loop iterates, the program sleeps before it clears the screen using ANSI escape codes and displays the updated stats. During the first iteration of the loop, the program also stores the initial CPU values to use for the CPU and memory usage calculations and begins displaying the memory and CPU values on the next iteration so that the program can compare the current CPU values to the initial CPU values. This is why the loop runs for the number of samples + 1. This function also sets up signal handlers for SIGINT and SIGTSTP to handle Ctrl-C and Ctrl-Z respectively. The signal handlers prompt the user if they want to quit the program and then exits the program if the user inputs 'y' or 'Y' for SIGINT. For SIGTSTP, the program prints a message that it is ignoring the signal since the program should not run in the background. Additionally, it creates pipes for communication between the parent and child processes and forks child processes to run the reporting functions. The parent process waits for the child processes to finish before continuing. This is done to ensure that the child processes have finished writing to the pipes before the parent process reads from them. The parent process then reads from the pipes and prints the data to the console. The parent process also frees the memory allocated for the memory and CPU arrays and closes the write ends of the pipes.

get_memory_info() - This function gets the memory information of the system. It uses sysinfo from the sys/sysinfo.h library to get the memory information of the system. The memory information includes the used physical memory, total physical memory, used virtual memory, and total virtual memory in GB. The physical memory is the actual RAM of the system, while the virtual memory is the combination of the RAM and the swap space. The function takes in an array of strings that stores the memory information, the current iteration, the previous virtual memory usage, the total number of samples, a flag for graphics, and a flag for sequential. It then calculates the used physical memory, total physical memory, used virtual memory, and total virtual memory. The function also calculates the difference between the current used virtual memory and the previous used virtual memory to get the change in memory usage. The function then stores the memory information in the array of strings and returns the array of strings. If the sequential flag is activated, the function only displays the current memory information, emptying the memory array of the previous values.
If the graphics flag is activated, the function prints '####* ' for positive changes in the used virtual memory, '::::@' for negative changes, '@' for -zero values, '*' for +zero values, and 'o' for a true zero. This is done to create a visual representation of the memory usage. The number of '#' or ':' depends on the decimal values of the used virtual memory difference. The changes in the used virtual memory are calculated by comparing the current used virtual memory to the previously used virtual memory, before rounding, meaning that values such as 0.0005 get represented as 0.00, creating an illusion of no change. This is why there is a distinction between +zero, -zero, and true zero values. The first displayed value, for example, we know is a true zero since it is the first, and will display 'o'.

report_connected_memory() - This function writes the memory information to a pipe. It takes in the pipe file descriptor, an array of strings that stores the memory information, the current iteration, the previous virtual memory usage, the total number of samples, a flag for graphics, and a flag for sequential. It then calls the get_memory_info() function to get the memory information and writes it to the pipe.

get_cpu_values() - This function reads the first line of /proc/stat to get the CPU usage. It then parses the line to get the values of the user, nice, system, idle, iowait, irq, and softirq. It then calculates the new cpu values and stores them in the int array 'cpu_totals'.

get_cpu_usage() - This function gets the CPU usage of the system. It takes in an array of strings 'cpu', the current iteration 'i', the initial cpu usage 'cpu_initial', the total number of samples 'total_samples', a flag for graphics 'graphics', and a flag for sequential 'sequential'. It uses get_cpu_values() to get the CPU usage and then calculates the CPU usage as a percentage. The function then stores the CPU usage in the array of strings and returns the array of strings. If the sequential flag is activated, the function only displays the current CPU information, emptying the cpu array of the previous values.
If the graphics flag is activated, the function prints '|||' to represent the CPU usage. The number of '|' depends on the decimal values of the CPU usage. The number of '|' is calculated by taking the floor of the CPU usage + 3.

report_connected_cpu() - This function writes the CPU information to a pipe. It takes in the pipe file descriptor, an array of strings that stores the CPU information, the current iteration, the initial CPU usage, the total number of samples, a flag for graphics, and a flag for sequential. It then calls the get_cpu_usage() function to get the CPU information and writes it to the pipe.

print_system_info() - This function prints the system information of the system. It uses uname from the sys/utsname.h library to get the system information and then prints it to the console. The system information includes the system name, machine name, version, release, architecture, and time the system has been running. The system name is the name of the operating system, the machine is the node name, the version is the release version, the release is the release date, the architecture is the machine information, and the time is the time the system has been running, shown in a few different ways.

get_users_usage() - This function gets the users and their sessions using getutent and utmp.h. It uses getutent() to get the user information by iterating through the utmp file. It then stores the user information in a string and returns the string.

report_connected_users() - This function writes the user information to a pipe. It takes in the pipe file descriptor and then calls the get_users_usage() function to get the user information and writes it to the pipe.

handle_sigint() - This function handles the SIGINT signal, which is sent when the user presses Ctrl-C. It prompts the user if they want to quit the program and then exits the program if the user inputs 'y' or 'Y'. If the user inputs any other character, the function prints a message that the program is continuing.

handle_sigtstp() - This function handles the SIGTSTP signal, which is sent when the user presses Ctrl-Z. It prints a message that the program is ignoring the signal since the program should not run in the background.


### Continued development

After finishing this project, I look forward to the next challenge. 

## Author

- LinkedIn - [Katherine Pravdin](https://www.linkedin.com/in/katherinepravdin)
