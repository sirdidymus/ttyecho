#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <unistd.h>
#include "util.h" 

char *prog_name;

void usage(){ 
    fprintf(stderr, "Usage: %s [-n] DEVNAME COMMAND\n" \
            "'-n' is an optional argument to place a newline " \
            "at the end of the command\n", prog_name);
    exit(1); 
} 

int main(int argc, char *argv[]){

     char *command; 
     char *newline = "\n"; 
     int opt, fd, nflag = 0, mem_len = 0; 
     prog_name = argv[0];

     while ((opt = getopt(argc, argv, ":n")) != -1){ 
       switch(opt){ 
        case 'n': 
             nflag = 1; 
             break; 
        case '?':
             printf("unknown argument(s):\n\n"); 
             usage(); 
             exit(EXIT_FAILURE); 
        default:
             usage(); 
             exit(EXIT_FAILURE); 
      }
     }; argc-=optind, argv += optind; 

     if (argc < 2) { 
        usage();
     } 

    if ((fd = open(argv[0], O_RDWR)) == -1){ 
        perror("open DEVICE");
        exit(EXIT_FAILURE); 
    }

    for (int i = 1; i < argc; i++){ 

        mem_len += strlen(argv[i]) + 2;
     
        if (i > 1) { 
            if ((command = realloc(command, mem_len)) == NULL){
                perror("realloc");
                exit(EXIT_FAILURE); 
            }
        } else { 
            if ((command = malloc(mem_len)) == NULL){ 
                perror("malloc");
                exit(EXIT_FAILURE); 
            }
        }
        
        strcat(command, argv[i]); 
        strcat(command, " "); 
    } 
     
    for (int i = 0; command[i]; i++){
         ioctl(fd, TIOCSTI, command + i);
    }

    if(nflag){
        ioctl(fd, TIOCSTI, newline);
    };  
    
    close(fd); 
    free(command);
    exit(EXIT_SUCCESS); 
}     
    
