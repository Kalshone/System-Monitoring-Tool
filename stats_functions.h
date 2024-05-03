// stats_functions.h

#ifndef STATS_FUNCTIONS_H
#define STATS_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <sys/wait.h>

void print_memory_usage();
char* get_memory_info(char* data, char *memory[], int i, double *prev_virtual, int total_samples, int graphics, int sequential);
void report_connected_memory(int pipe_fd, char *memory[], int i, double *prev_virtual, int total_samples, int graphics, int sequential);
void get_cpu_values(int cpu_totals[]);
char* get_cpu_usage(char* data, char *cpu[], int i, int cpu_initial[], int total_samples, int graphics, int sequential);
void report_connected_cpu(int pipe_fd, char *cpu[], int i, int cpu_initial[], int total_samples, int graphics, int sequential);
void print_system_info();
char* get_users_usage(char* data);
void report_connected_users(int pipe_fd);
void handle_sigint(int sig);
void handle_sigtstp(int sig);

#endif /* STATS_FUNCTIONS_H */
