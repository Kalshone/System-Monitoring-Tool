// Assigment 3: System Monitoring Tool
// Katherine Pravdin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h> // https://man7.org/linux/man-pages/man0/sys_resource.h.0p.html
#include <sys/utsname.h>  // https://man7.org/linux/man-pages/man2/uname.2.html
#include <sys/sysinfo.h> //  https://man7.org/linux/man-pages/man2/sysinfo.2.html
#include <sys/types.h>  //  https://man7.org/linux/man-pages/man0/sys_types.h.0p.html
#include <utmp.h>  //  https://man7.org/linux/man-pages/man5/utmp.5.html
#include <unistd.h>  // https://man7.org/linux/man-pages/man0/unistd.h.0p.html
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <sys/wait.h>

void print_memory_usage(){
    // Prints the memory usage of the program using getrusage from sys/resource.h
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}

char* get_memory_info(char* data, char *memory[], int i, double *prev_virtual, int total_samples, int graphics, int sequential){
    // Gets the memory usage of the system. 
    // Takes in an array of strings 'memory', the current iteration 'i', the previous virtual memory usage 
    // 'prev_virtual' as a pointer, the total number of samples 'total_samples', a flag for graphics 'graphics', 
    // and a flag for sequential 'sequential'. Uses sysinfo from sys/sysinfo.h to get the memory usage of the system.
    if (i >= 0){
        struct sysinfo info;
        sysinfo(&info);

        double used_physical = (double)info.totalram - (double)info.freeram - (double)info.bufferram;
        double total_physical = (double)info.totalram - (double)info.bufferram;
        double used_virtual = ((double)(info.totalswap) + (double)(info.totalram)) - ((double)(info.freeswap) + (double)(info.freeram));
        double total_virtual = ((double)(info.totalswap) + (double)(info.totalram)) - (double)(info.bufferram);
        double dif = used_virtual * 1e-9 - *prev_virtual;

        char used_virtual_str[50];
        sprintf(used_virtual_str, "%f", used_virtual * 1e-9);
        strcpy(data, used_virtual_str);
        strcat(data, "\n");

        if (sequential == 1){
            for(int k = 0; k < total_samples; k++){
                memory[k][0] = '\0';
            }
        }

        if (graphics == 1){
            char tags[300];
            tags[0] = '\0';

            if (i == 0){
                dif = 0.00;
            } 

            char dif_str[] = "00.00";
            sprintf(dif_str, "%0.2f", dif);

            char decimal_str_no_dot[3];
            strncpy(decimal_str_no_dot, strchr(dif_str, '.') + 1, 3);
            int decimal_int =  strtol(decimal_str_no_dot, (char **)NULL, 10);

            for(int j = 0; j < decimal_int + 1; j++){
                if (dif > 0){
                    tags[j] = '#';
                    if (j == decimal_int){
                        tags[j] = '*';
                    }
                } else if (dif < 0){
                    tags[j] = ':';
                    if (j == decimal_int){
                        tags[j] = '@';
                    }
                } else if(dif == 0){
                    tags[j] = 'o';
                } 
            }

            tags[decimal_int + 1] = '\0';
            
            sprintf(memory[i], "%0.2f GB / %0.2f GB -- %0.2f GB / %0.2f GB  |%s %0.2f (%0.2f)",  
                    used_physical * 1e-9, total_physical * 1e-9, used_virtual * 1e-9, total_virtual * 1e-9,
                    tags, dif, used_virtual * 1e-9);
            
        } else {
            sprintf(memory[i], "%0.2f GB / %0.2f GB -- %0.2f GB / %0.2f GB",  used_physical * 1e-9, 
                total_physical * 1e-9, used_virtual * 1e-9, total_virtual * 1e-9);
        } 
    } else{
        strcpy(data, "### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
    }


    strcat(data, "### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
    for (int j = 0; j < total_samples; j++) {
        strcat(data, memory[j]);
        strcat(data, " \n");
    }
    strcat(data, "---------------------------------------\n");

    // printf("Data: %s\n", data);

    return data;
}

void report_connected_memory(int pipe_fd, char *memory[], int i, double *prev_virtual, int total_samples, int graphics, int sequential) {
    // char *data = (char*)malloc(2000 * sizeof(char));
    // memset(data, 0, 2000 * sizeof(char));

    // get_memory_info(memory, i, prev_virtual, total_samples, graphics, sequential);
    
    // // strcpy(data, "### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
    // // for (int j = 0; j < total_samples; j++) {
    // //     strcat(data, memory[j]);
    // //     strcat(data, " \n");
    // // }
    // // strcat(data, "---------------------------------------\n");

    

    // write(pipe_fd, data, strlen(data));
    // free(data);

    
    
    char *data = (char*)malloc(2000 * sizeof(char));
    memset(data, 0, 2000 * sizeof(char));
    
    data = get_memory_info(data, memory, i, prev_virtual, total_samples, graphics, sequential);

    // printf("Data: %s\n", data);

    write(pipe_fd, data, strlen(data));

    free(data);
}

void get_cpu_values(int cpu_totals[]){
    // Gets the total cpu usage of the system using /proc/stat and stores the idle and the total
    // in the int array 'cpu_totals'.
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Error opening /proc/stat");
    }
    read = getline(&line, &len, file);
    fclose(file);

    if (read == -1) {
        fprintf(stderr, "Error reading /proc/stat\n");
    } 

    int counter = 0;
    int total = 0;
    int idle = 0;
    char *token = strtok(line, " ");

    while (counter <= 7 && token != NULL) {
        if (counter != 0){
            total += atoi(token);
            if(counter == 4){
                idle = atoi(token);
            }
        }
        token = strtok(NULL, " ");
        counter++;
    }

    cpu_totals[0] = idle;
    cpu_totals[1] = total;
}

char* get_cpu_usage(char* data, char *cpu[], int i, int cpu_initial[], int total_samples, int graphics, int sequential){
    // Prints the cpu usage of the system using get_cpu_values and /proc/stat. Takes in an array of strings 
    // 'cpu', the current iteration 'i', the initial cpu usage 'cpu_initial', the total number of samples
    // 'total_samples', a flag for graphics 'graphics', and a flag for sequential 'sequential'. 
    // Uses sysconf from unistd.h to get the number of cores. 

    // printf("Number of cores: %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
    strcpy(data, "Number of cores: ");
    // strcat(data, sysconf(_SC_NPROCESSORS_ONLN));
    char num_processors[50];
    sprintf(num_processors, "%ld", sysconf(_SC_NPROCESSORS_ONLN));
    strcat(data, num_processors);
    strcat(data, "\n");

    if (i >= 0){
        int cpu_totals[2];
        get_cpu_values(cpu_totals);

        int T1 = cpu_initial[1];
        int T2 = cpu_totals[1];
        int U1 = T1 - cpu_initial[0];
        int U2 = T2 - cpu_totals[0];
        int total_dif = T2 - T1;
        int idle_dif = U2 - U1;

        double cpu_percentage = ((double)idle_dif / (double)total_dif) * 100;

        // check if it is nan
        if (isnan(cpu_percentage)){
            cpu_percentage = 0.00;
        }

        if (sequential == 1){
            for(int j = 0; j < total_samples; j++){
                cpu[j][0] = '\0';
            }
        }

        char temp1[100];
        snprintf(temp1, sizeof(temp1), " total cpu use = %0.2f%%\n", cpu_percentage);
        strncat(data, temp1, strlen(temp1));
        
        if (graphics == 1){
            char lines[(int)cpu_percentage + 3 + 1];
            for(int k = 0; k < (int)cpu_percentage + 3 + 1; k++){
                lines[k] = '|';
            }
            if (cpu_percentage == 0.00){
                lines[(int)cpu_percentage + 2] = '\0';
            } else {
                lines[(int)cpu_percentage + 3] = '\0';
            }
            
            sprintf(cpu[i], "%s %.2f", lines, cpu_percentage);

            for(int l = 0; l < total_samples; l++){
                char temp2[100];
                snprintf(temp2, sizeof(temp2), "%s\n", cpu[l]);
                strncat(data, temp2, strlen(temp2));
            }
        }
    }

    strcat(data, "---------------------------------------\n");
    return data;
}

void report_connected_cpu(int pipe_fd, char *cpu[], int i, int cpu_initial[], int total_samples, int graphics, int sequential) {
    char *data = (char*)malloc(2000 * sizeof(char));
    memset(data, 0, 2000 * sizeof(char));
    
    data = get_cpu_usage(data, cpu, i, cpu_initial, total_samples, graphics, sequential);

    write(pipe_fd, data, strlen(data));

    free(data);
}

void print_system_info(){
    // Prints the system information using uname from sys/utsname.h and sysinfo from sys/sysinfo.h.
    struct utsname sys_info;
    uname(&sys_info);

    printf("### System Information ###\n");
    printf("System Name = %s\n", sys_info.sysname); 
    printf("Machine Name = %s\n", sys_info.nodename);
    printf("Version = %s\n", sys_info.version); 
    printf("Release = %s\n", sys_info.release); 
    printf("Architecture = %s\n", sys_info.machine);
    
    struct sysinfo info;
    sysinfo(&info);

    int days = info.uptime / 86400;
    int hours = (info.uptime - (days * 86400)) / 3600;
    int minutes = (info.uptime - (days * 86400) - (hours * 3600)) / 60;
    int seconds = (info.uptime - (days * 86400) - (hours * 3600) - (minutes * 60));

    printf("System running since last reboot: %d days %d:%d:%d (%d:%d:%d)\n", 
        days, hours, minutes, seconds, hours + (days * 24), minutes, seconds);
    
    printf("---------------------------------------\n");
}

char* get_users_usage(char* data){
    // gets the users and their sessions using getutent and utmp.h.
    struct utmp *user_info;
    utmpname(_PATH_UTMP);
	setutent();
    user_info = getutent(); 

    strcpy(data, "### Sessions/Users ###\n");  

    while(user_info != NULL){
        if(user_info->ut_type==USER_PROCESS){ 
            char temp[328];
            snprintf(temp, sizeof(temp), "%s       %s %s\n", user_info->ut_user, user_info->ut_line, user_info->ut_host);
            strncat(data, temp, strlen(temp));
        }
        user_info = getutent();
    }

    strcat(data, "---------------------------------------\n");

    endutent();
    free(user_info);

    return data;
}

void report_connected_users(int pipe_fd) {
    char *data = (char*)malloc(2000 * sizeof(char));
    memset(data, 0, 2000 * sizeof(char));
    
    data = get_users_usage(data);

    write(pipe_fd, data, strlen(data));

    free(data);
}


// Function to handle Ctrl-C (SIGINT)
// write(pipe_fd, *data, sizeof(*data));
void handle_sigint(int sig) {
    char choice;
    printf("\n\nDo you really want to quit? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        exit(0);
    } else {
        printf("Continuing the program...\n");
    }
}

// Function to handle Ctrl-Z (SIGTSTP)
void handle_sigtstp(int sig) {
    printf("\n\nIgnoring Ctrl-Z since the program should not run in the background.\n");
}