#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_LEN 128

void p1(int reader, int writer, FILE *input, pid_t cpid);
void p2(int reader, int writer, FILE *input);
void initialPrint(FILE * fp);

int main(int argc, char *argv[]){
    
    int fd1[2];
    int fd2[2];
    pid_t pid;

    if(argc != 2){
        fprintf(stderr, "Wrong parameters number.\n");
        exit(1);
    }

    FILE *input = fopen(argv[1], "r+");

    if(input == NULL){
        fprintf(stderr, "Error in file opening.\n");
        exit(1);
    }

    if(pipe(fd1) != 0 || pipe(fd2) != 0){
        fprintf(stderr, "Error in opening pipe.\n");
        exit(1);
    }

    pid = fork();

    if(pid){
        close(fd1[0]);
        close(fd2[1]);
        p1(fd2[0], fd1[1], input, pid);
        wait(NULL);
    }
    else{
        close(fd1[1]);
        close(fd2[0]);
        p2(fd1[0], fd2[1], input);
    }
    
    return 0;

}

void initialPrint(FILE * fp){
    char row[MAX_LEN];

    fseek(fp, 0, SEEK_SET);

    while(fgets(row, MAX_LEN, fp) != NULL){
        printf("%s", row);
    }

    printf("\n");

}

void p1(int reader, int writer, FILE * input, pid_t cpid){

    char row[MAX_LEN];
    int flag1 = 1, flag2 = 1;

    while(flag1){
        
        printf("Parent process is awake! Here's my child's poem:\n\n");

        initialPrint(input);
        flag2=1;

        printf("Now please insert my poem:\n");

        while(flag2){
            fgets(row, MAX_LEN, stdin);

            if(strcmp(row, "end\n") ==0){
                flag2=0;
                printf("Parent process is tired of poetry, it goes to sleep...\n");
            }
            else if(strcmp(row, "END\n")==0){
                printf("We're all tired of poetry, terminating all processes...\n");
                flag2 = flag1 = 0;
                kill(cpid, SIGKILL);
            }
            else{
                fputs(row, input);
            }
        }

        write(writer, "a", sizeof(char));
        read(reader, NULL, sizeof(char));

    }

    return;

}

void p2(int reader, int writer, FILE *input){

    char row[MAX_LEN];
    int flag1 = 1, flag2 = 1;

    while(flag1){
        read(reader, NULL, sizeof(char));

        printf("Child process is awake! Here's my parent's poem:\n\n");

        initialPrint(input);
        flag2=1;

        printf("Now please insert my poem:\n");

        while(flag2){
            fgets(row, MAX_LEN, stdin);

            if(strcmp(row, "end\n")==0){
                flag2=0;
                printf("Child process is tired of poetry, it goes to sleep...\n");
            }
            else if(strcmp(row, "END\n")==0){
                printf("We're all tired of poetry, terminating all processes...\n");
                flag2 = flag1 = 0;
                kill(getppid(), SIGKILL);
            }
            else{
                fputs(row, input);
            }
        }


        write(writer, "a", sizeof(char));
    }

    return;

}