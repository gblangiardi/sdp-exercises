#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define BUFFER_SIZE 16

void *prodF(void *par);
void *consF(void *par);

typedef struct ts{

    sem_t *full;
    sem_t *empty;

    int buffer[BUFFER_SIZE];
    int tail;
    int head;
    
} threadstruct;

int main(void){
    pthread_t *tid1, *tid2;
    int rc1, rc2;
    
    threadstruct *str = (threadstruct *)malloc(sizeof(threadstruct));
    tid1 = (pthread_t *)malloc(sizeof(pthread_t));
    tid2 = (pthread_t *)malloc(sizeof(pthread_t));

    if(tid1 == NULL || tid2 == NULL || str == NULL){
        fprintf(stderr, "Memory allocation error.\n");
        return -1;
    }

    str->head = str->tail = 0;
    str->full = (sem_t *)malloc(sizeof(sem_t));
    str->empty = (sem_t *)malloc(sizeof(sem_t));

    if(str->full == NULL || str->empty == NULL){
        fprintf(stderr, "Memory allocation error.\n");
        return -1;
    }

    sem_init(str->full, 0, 0);
    sem_init(str->empty, 0, BUFFER_SIZE);

    rc1 = pthread_create(tid1, NULL, prodF, (void *) str);
    rc2 = pthread_create(tid2, NULL, consF, (void *) str);
    
    if(rc1 != 0 || rc2 != 0){
        fprintf(stderr, "Thread creation error.\n");
        return -1;
    }

    pthread_join(*tid1, NULL);
    pthread_join(*tid2, NULL);

    sem_destroy(str->full);
    sem_destroy(str->empty);

    free(str);
    free(tid1);
    free(tid2);

    return 0;
}

void *prodF(void *par){
    threadstruct *str = (threadstruct *) par;

    int i;


    for(i=0; i<=1000; i++){
        sem_wait(str->empty);

        if(i == 1000){
            str->buffer[str->tail] = -1;
            str->tail = (str->tail +1)%BUFFER_SIZE; 
        }
        else{
            str->buffer[str->tail] = i;
            str->tail = (str->tail +1)%BUFFER_SIZE;
        }

        sem_post(str->full);
    }

    pthread_exit((void*) 0);
    
}

void *consF(void *par){
    FILE *fd;
    threadstruct *str = (void *)par;
    int val=0;
    int flag=1;

    fd = fopen("out.txt", "w");

    if(fd == NULL){
        fprintf(stderr, "Error in file opening.\n");
        pthread_exit((void *)-1);
    }

    while(flag){

        sem_wait(str->full);
        
        val = str->buffer[str->head];
        str->head = (str->head +1)%BUFFER_SIZE;

        sem_post(str->empty);

        if(val != -1){
            fprintf(fd, "%d\n", val);
        }
        else
            flag = 0;
    }

    fclose(fd);

    pthread_exit((void *)0);
}
