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

// add stats_functions.c
#include "stats_functions.h"

// void print_memory_usage(){
//     // Prints the memory usage of the program using getrusage from sys/resource.h
//     struct rusage usage;
//     getrusage(RUSAGE_SELF, &usage);

//     printf("Memory usage: %ld KB\n", usage.ru_maxrss);
// }

// char* get_memory_info(char* data, char *memory[], int i, double *prev_virtual, int total_samples, int graphics, int sequential){
//     // Gets the memory usage of the system. 
//     // Takes in an array of strings 'memory', the current iteration 'i', the previous virtual memory usage 
//     // 'prev_virtual' as a pointer, the total number of samples 'total_samples', a flag for graphics 'graphics', 
//     // and a flag for sequential 'sequential'. Uses sysinfo from sys/sysinfo.h to get the memory usage of the system.
//     if (i >= 0){
//         struct sysinfo info;
//         sysinfo(&info);

//         double used_physical = (double)info.totalram - (double)info.freeram - (double)info.bufferram;
//         double total_physical = (double)info.totalram - (double)info.bufferram;
//         double used_virtual = ((double)(info.totalswap) + (double)(info.totalram)) - ((double)(info.freeswap) + (double)(info.freeram));
//         double total_virtual = ((double)(info.totalswap) + (double)(info.totalram)) - (double)(info.bufferram);
//         double dif = used_virtual * 1e-9 - *prev_virtual;

//         char used_virtual_str[50];
//         sprintf(used_virtual_str, "%f", used_virtual * 1e-9);
//         strcpy(data, used_virtual_str);
//         strcat(data, "\n");

//         if (sequential == 1){
//             for(int k = 0; k < total_samples; k++){
//                 memory[k][0] = '\0';
//             }
//         }

//         if (graphics == 1){
//             char tags[300];
//             tags[0] = '\0';

//             if (i == 0){
//                 dif = 0.00;
//             } 

//             char dif_str[] = "00.00";
//             sprintf(dif_str, "%0.2f", dif);

//             char decimal_str_no_dot[3];
//             strncpy(decimal_str_no_dot, strchr(dif_str, '.') + 1, 3);
//             int decimal_int =  strtol(decimal_str_no_dot, (char **)NULL, 10);

//             for(int j = 0; j < decimal_int + 1; j++){
//                 if (dif > 0){
//                     tags[j] = '#';
//                     if (j == decimal_int){
//                         tags[j] = '*';
//                     }
//                 } else if (dif < 0){
//                     tags[j] = ':';
//                     if (j == decimal_int){
//                         tags[j] = '@';
//                     }
//                 } else if(dif == 0){
//                     tags[j] = 'o';
//                 } 
//             }

//             tags[decimal_int + 1] = '\0';
            
//             sprintf(memory[i], "%0.2f GB / %0.2f GB -- %0.2f GB / %0.2f GB  |%s %0.2f (%0.2f)",  
//                     used_physical * 1e-9, total_physical * 1e-9, used_virtual * 1e-9, total_virtual * 1e-9,
//                     tags, dif, used_virtual * 1e-9);
            
//         } else {
//             sprintf(memory[i], "%0.2f GB / %0.2f GB -- %0.2f GB / %0.2f GB",  used_physical * 1e-9, 
//                 total_physical * 1e-9, used_virtual * 1e-9, total_virtual * 1e-9);
//         } 
//     } else{
//         strcpy(data, "### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
//     }


//     strcat(data, "### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
//     for (int j = 0; j < total_samples; j++) {
//         strcat(data, memory[j]);
//         strcat(data, " \n");
//     }
//     strcat(data, "---------------------------------------\n");

//     // printf("Data: %s\n", data);

//     return data;
// }

// void report_connected_memory(int pipe_fd, char *memory[], int i, double *prev_virtual, int total_samples, int graphics, int sequential) {
//     // char *data = (char*)malloc(2000 * sizeof(char));
//     // memset(data, 0, 2000 * sizeof(char));

//     // get_memory_info(memory, i, prev_virtual, total_samples, graphics, sequential);
    
//     // // strcpy(data, "### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
//     // // for (int j = 0; j < total_samples; j++) {
//     // //     strcat(data, memory[j]);
//     // //     strcat(data, " \n");
//     // // }
//     // // strcat(data, "---------------------------------------\n");

    

//     // write(pipe_fd, data, strlen(data));
//     // free(data);

    
    
//     char *data = (char*)malloc(2000 * sizeof(char));
//     memset(data, 0, 2000 * sizeof(char));
    
//     data = get_memory_info(data, memory, i, prev_virtual, total_samples, graphics, sequential);

//     // printf("Data: %s\n", data);

//     write(pipe_fd, data, strlen(data));

//     free(data);
// }

// void get_cpu_values(int cpu_totals[]){
//     // Gets the total cpu usage of the system using /proc/stat and stores the idle and the total
//     // in the int array 'cpu_totals'.
//     FILE *file;
//     char *line = NULL;
//     size_t len = 0;
//     ssize_t read;
//     file = fopen("/proc/stat", "r");
//     if (file == NULL) {
//         perror("Error opening /proc/stat");
//     }
//     read = getline(&line, &len, file);
//     fclose(file);

//     if (read == -1) {
//         fprintf(stderr, "Error reading /proc/stat\n");
//     } 

//     int counter = 0;
//     int total = 0;
//     int idle = 0;
//     char *token = strtok(line, " ");

//     while (counter <= 7 && token != NULL) {
//         if (counter != 0){
//             total += atoi(token);
//             if(counter == 4){
//                 idle = atoi(token);
//             }
//         }
//         token = strtok(NULL, " ");
//         counter++;
//     }

//     cpu_totals[0] = idle;
//     cpu_totals[1] = total;
// }

// char* get_cpu_usage(char* data, char *cpu[], int i, int cpu_initial[], int total_samples, int graphics, int sequential){
//     // Prints the cpu usage of the system using get_cpu_values and /proc/stat. Takes in an array of strings 
//     // 'cpu', the current iteration 'i', the initial cpu usage 'cpu_initial', the total number of samples
//     // 'total_samples', a flag for graphics 'graphics', and a flag for sequential 'sequential'. 
//     // Uses sysconf from unistd.h to get the number of cores. 

//     // printf("Number of cores: %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
//     strcpy(data, "Number of cores: ");
//     // strcat(data, sysconf(_SC_NPROCESSORS_ONLN));
//     char num_processors[50];
//     sprintf(num_processors, "%ld", sysconf(_SC_NPROCESSORS_ONLN));
//     strcat(data, num_processors);
//     strcat(data, "\n");

//     if (i >= 0){
//         int cpu_totals[2];
//         get_cpu_values(cpu_totals);

//         int T1 = cpu_initial[1];
//         int T2 = cpu_totals[1];
//         int U1 = T1 - cpu_initial[0];
//         int U2 = T2 - cpu_totals[0];
//         int total_dif = T2 - T1;
//         int idle_dif = U2 - U1;

//         double cpu_percentage = ((double)idle_dif / (double)total_dif) * 100;

//         // check if it is nan
//         if (isnan(cpu_percentage)){
//             cpu_percentage = 0.00;
//         }

//         if (sequential == 1){
//             for(int j = 0; j < total_samples; j++){
//                 cpu[j][0] = '\0';
//             }
//         }

//         char temp1[100];
//         snprintf(temp1, sizeof(temp1), " total cpu use = %0.2f%%\n", cpu_percentage);
//         strncat(data, temp1, strlen(temp1));
        
//         if (graphics == 1){
//             char lines[(int)cpu_percentage + 3 + 1];
//             for(int k = 0; k < (int)cpu_percentage + 3 + 1; k++){
//                 lines[k] = '|';
//             }
//             if (cpu_percentage == 0.00){
//                 lines[(int)cpu_percentage + 2] = '\0';
//             } else {
//                 lines[(int)cpu_percentage + 3] = '\0';
//             }
            
//             sprintf(cpu[i], "%s %.2f", lines, cpu_percentage);

//             for(int l = 0; l < total_samples; l++){
//                 char temp2[100];
//                 snprintf(temp2, sizeof(temp2), "%s\n", cpu[l]);
//                 strncat(data, temp2, strlen(temp2));
//             }
//         }
//     }

//     strcat(data, "---------------------------------------\n");
//     return data;
// }

// void report_connected_cpu(int pipe_fd, char *cpu[], int i, int cpu_initial[], int total_samples, int graphics, int sequential) {
//     char *data = (char*)malloc(2000 * sizeof(char));
//     memset(data, 0, 2000 * sizeof(char));
    
//     data = get_cpu_usage(data, cpu, i, cpu_initial, total_samples, graphics, sequential);

//     write(pipe_fd, data, strlen(data));

//     free(data);
// }

// void print_system_info(){
//     // Prints the system information using uname from sys/utsname.h and sysinfo from sys/sysinfo.h.
//     struct utsname sys_info;
//     uname(&sys_info);

//     printf("### System Information ###\n");
//     printf("System Name = %s\n", sys_info.sysname); 
//     printf("Machine Name = %s\n", sys_info.nodename);
//     printf("Version = %s\n", sys_info.version); 
//     printf("Release = %s\n", sys_info.release); 
//     printf("Architecture = %s\n", sys_info.machine);
    
//     struct sysinfo info;
//     sysinfo(&info);

//     int days = info.uptime / 86400;
//     int hours = (info.uptime - (days * 86400)) / 3600;
//     int minutes = (info.uptime - (days * 86400) - (hours * 3600)) / 60;
//     int seconds = (info.uptime - (days * 86400) - (hours * 3600) - (minutes * 60));

//     printf("System running since last reboot: %d days %d:%d:%d (%d:%d:%d)\n", 
//         days, hours, minutes, seconds, hours + (days * 24), minutes, seconds);
    
//     printf("---------------------------------------\n");
// }

// char* get_users_usage(char* data){
//     // gets the users and their sessions using getutent and utmp.h.
//     struct utmp *user_info;
//     utmpname(_PATH_UTMP);
// 	setutent();
//     user_info = getutent(); 

//     strcpy(data, "### Sessions/Users ###\n");  

//     while(user_info != NULL){
//         if(user_info->ut_type==USER_PROCESS){ 
//             char temp[328];
//             snprintf(temp, sizeof(temp), "%s       %s %s\n", user_info->ut_user, user_info->ut_line, user_info->ut_host);
//             strncat(data, temp, strlen(temp));
//         }
//         user_info = getutent();
//     }

//     strcat(data, "---------------------------------------\n");

//     endutent();
//     free(user_info);

//     return data;
// }

// void report_connected_users(int pipe_fd) {
//     char *data = (char*)malloc(2000 * sizeof(char));
//     memset(data, 0, 2000 * sizeof(char));
    
//     data = get_users_usage(data);

//     write(pipe_fd, data, strlen(data));

//     free(data);
// }


// // Function to handle Ctrl-C (SIGINT)
// // write(pipe_fd, *data, sizeof(*data));
// void handle_sigint(int sig) {
//     char choice;
//     printf("\n\nDo you really want to quit? (y/n): ");
//     scanf(" %c", &choice);
//     if (choice == 'y' || choice == 'Y') {
//         exit(0);
//     } else {
//         printf("Continuing the program...\n");
//     }
// }

// // Function to handle Ctrl-Z (SIGTSTP)
// void handle_sigtstp(int sig) {
//     printf("\n\nIgnoring Ctrl-Z since the program should not run in the background.\n");
// }

int main(int argc, char *argv[]) {
    // Main function that takes in command line arguments and prints the system information, memory usage,
    // cpu usage, and user sessions using the functions defined above

    // default values
    int total_samples = 10;
    int total_samples_set = 0;
    int system = 0;
    int user = 0;
    int delay = 1;
    int graphics = 0;
    int sequential = 0;
    int BUFFER_SIZE = 1000;

    // look at the arguments, see what needs to be displayed
    for (int i = 1; i < argc; i++) {
    // char *equals_ptr;
        // int equals_index;

        if (strcmp(argv[i], "--system") == 0){
            system = 1;
        } else if (strcmp(argv[i], "--user") == 0){
            user = 1;
        } else if ((strcmp(argv[i], "--graphics") == 0) || (strcmp(argv[i], "-g") == 0)){
            graphics = 1;
        } else if (strcmp(argv[i], "--sequential") == 0){
            sequential = 1;
        } else if (strstr(argv[i], "--tdelay=") != NULL || (isdigit(argv[i][0]) && total_samples_set == 1)){
            if (isdigit(argv[i][0])){
                delay = (int) strtol(argv[i], (char **)NULL, 10);
            } else {
                delay = atoi(strchr(argv[i], '=') + 1);
            }

        } else if ((strstr(argv[i], "--samples=") != NULL) || isdigit(argv[i][0])){
            if (isdigit(argv[i][0])){
                total_samples = (int) strtol(argv[i], (char **)NULL, 10);
            } else {
                total_samples = atoi(strchr(argv[i], '=') + 1);
            }
            total_samples_set = 1;
        } else{
            printf("Invalid argument: %s\n", argv[i]);
            printf("Usage: ./mySystemStats [--system] [--user] [--graphics] [--sequential] [--tdelay=N OR N] [--samples=N OR N]\n");
            return 1;
        }
    }

    // create arrays to store the memory and cpu usage
    char* memory[total_samples];
    char* cpu[total_samples];

    // Allocate memory for each string and initialize them as empty
    for (int i = 0; i < total_samples; i++) {
        memory[i] = (char*)malloc((300 + 1) * sizeof(char)); // account for null terminator
        memory[i][0] = '\0'; // set as empty

        cpu[i] = (char*)malloc((300 + 1) * sizeof(char)); 
        cpu[i][0] = '\0'; 
    }

    // Setting up signal handlers
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);

    // // Pipes for communication
    // int users_pipe[2];
    int mem_pipe[2], users_pipe[2], cpu_pipe[2];
    pipe(mem_pipe);
    pipe(users_pipe);
    pipe(cpu_pipe);
    // pid_t users_pid;
    pid_t mem_pid, users_pid, cpu_pid;

    // define the initial cpu usage for reference
    double prev_virtual = 0;
    int cpu_initial[2];
    get_cpu_values(cpu_initial);

    // start printing info:
    for (int i = 0; i < total_samples + 1; i++) {
        if (sequential == 0){
            printf("\033[2J\033[1;1H");
        } else {
            printf("Iteration: %d\n", i);
        }

        printf("Nbr of samples: %d -- every %d seconds\n", total_samples, delay);
        print_memory_usage();
        printf("---------------------------------------\n");

        users_pid = fork();
        if (users_pid == 0) {
            close(users_pipe[0]); // Close read end of users pipe
            report_connected_users(users_pipe[1]);
            exit(0);
        }

        mem_pid = fork();
        if (mem_pid == 0) {
            close(mem_pipe[0]); // Close read end of memory pipe
            report_connected_memory(mem_pipe[1], memory, i - 1, &prev_virtual, total_samples, graphics, sequential);
            exit(0);
        } 

        cpu_pid = fork();
        if (cpu_pid == 0) {
            close(cpu_pipe[0]); // Close read end of CPU pipe
            report_connected_cpu(cpu_pipe[1], cpu, i - 1, cpu_initial, total_samples, graphics, sequential);
            exit(0);
        }  

        // if (system == 1 && user == 0){ // EDIT
        //     print_memory_info(memory, i - 1, &prev_virtual, total_samples, graphics, sequential);
        //     print_cpu_usage(cpu, i - 1, cpu_initial, total_samples, graphics, sequential);
        // } else if (user == 1 && system == 0){
        //     print_users_usage();
        // } else {  
        //     print_memory_info(memory, i - 1, &prev_virtual, total_samples, graphics, sequential);
        //     print_users_usage();
        //     print_cpu_usage(cpu, i - 1, cpu_initial, total_samples, graphics, sequential);
        // }
        
        waitpid(users_pid, NULL, 0);
        waitpid(mem_pid, NULL, 0);
        waitpid(cpu_pid, NULL, 0);
        
        char buffer[BUFFER_SIZE];
        if (system == 1 && user == 0){
            memset(buffer, 0, BUFFER_SIZE);
            read(mem_pipe[0], buffer, BUFFER_SIZE);

            // buffer[BUFFER_SIZE] = '\0';
            char *line = strtok(buffer, "\n");

            int counter = 0;
            while (line != NULL) {
                if (counter > 0){
                    printf("%s\n", line);
                }
                if (counter-1 == i && i != 0){
                    strcpy(memory[i-1], line);
                }
                if (counter == 0){
                    prev_virtual = atof(line);
                }

                // Get the next line
                line = strtok(NULL, "\n");
                counter++;
            }

            memset(buffer, 0, BUFFER_SIZE);
            read(cpu_pipe[0], buffer, BUFFER_SIZE);

            // buffer[BUFFER_SIZE] = '\0';
            line = strtok(buffer, "\n");

            counter = 0;
            while (line != NULL) {
                if (counter <= 1 || line[0] == '-'){
                    printf("%s\n", line);
                } else{
                    printf("%4s%s\n", "", line);
                }

                if (counter-1 == i && i != 0){
                    strcpy(cpu[i-1], line);
                }

                line = strtok(NULL, "\n");
                counter++;
            }
        } else if (user == 1 && system == 0){
            memset(buffer, 0, BUFFER_SIZE);
            read(users_pipe[0], buffer, BUFFER_SIZE);
            printf("%s", buffer);
        } else {  
            memset(buffer, 0, BUFFER_SIZE);
            read(mem_pipe[0], buffer, BUFFER_SIZE);

            // buffer[BUFFER_SIZE] = '\0';
            char *line = strtok(buffer, "\n");

            int counter = 0;
            while (line != NULL) {
                if (counter > 0){
                    printf("%s\n", line);
                }
                if (counter-1 == i && i != 0){
                    strcpy(memory[i-1], line);
                }
                if (counter == 0){
                    prev_virtual = atof(line);
                }

                // Get the next line
                line = strtok(NULL, "\n");
                counter++;
            }


            memset(buffer, 0, BUFFER_SIZE);
            read(users_pipe[0], buffer, BUFFER_SIZE);
            printf("%s", buffer);


            memset(buffer, 0, BUFFER_SIZE);
            read(cpu_pipe[0], buffer, BUFFER_SIZE);

            // buffer[BUFFER_SIZE] = '\0';
            line = strtok(buffer, "\n");

            counter = 0;
            while (line != NULL) {
                // printf("%s\n", line);

                if (counter <= 1 || line[0] == '-'){
                    printf("%s\n", line);
                } else{
                    printf("%4s%s\n", "", line);
                }

                if (counter-1 == i && i != 0){
                    strcpy(cpu[i-1], line);
                }

                line = strtok(NULL, "\n");
                counter++;
            }
        }

        print_system_info();

        if (i < total_samples){
            usleep(delay*1000000);
        }
        // sleep(delay);
    }

    for (int i = 0; i < total_samples; i++) {
        free(memory[i]);
        free(cpu[i]);
    }

    // Forking child processes for reporting functions
    close(mem_pipe[1]); // Close write end of memory pipe
    close(users_pipe[1]); // Close write end of users pipe
    close(cpu_pipe[1]); // Close write end of CPU pipe

    // Waiting for child processes to finish
    waitpid(mem_pid, NULL, 0);
    waitpid(users_pid, NULL, 0);
    waitpid(cpu_pid, NULL, 0);
    
    return 0;
}
