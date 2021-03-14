#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct ts{
    long int *tids;
    int n;
    int i;
}threadstr;

void *threadfunct(void *par);

int main(int argc, char *argv[]){

    if(argc != 2){
        fprintf(stderr, "Wrong parameter number, usage is: %s n\n", argv[0]);
        return -1;
    }

    int n = atoi(argv[1]);
    int rc;
    pthread_t tid1, tid2;
    void *status;
    
    threadstr str1, str2;

    str1.i = str2.i = 0;
    str1.n = str2.n = n;
    str1.tids = (long int *)malloc(n*sizeof(long int));
    str2.tids = (long int *)malloc(n*sizeof(long int));

    if(str1.tids == NULL || str2.tids == NULL){
        fprintf(stderr, "Memory allocation error.\n");
        return -1;
    }

    str1.tids[str1.i] = str2.tids[str2.i] = (long int)pthread_self();

    rc = pthread_create(&tid1,NULL, threadfunct, (void *) &str1);
    
    if(rc!=0){
        fprintf(stderr, "Error on thread creation with code %d", rc);
        return rc;
    }

    rc = pthread_create(&tid2, NULL, threadfunct, (void *) &str2);

    if(rc!=0){
        fprintf(stderr, "Error on thread creation with code %d", rc);
        return rc;
    }

    pthread_join(tid1, &status);
    pthread_join(tid2, &status);

    free(str1.tids);
    free(str2.tids);

    return 0;
}

void *threadfunct(void *par){
    threadstr *str = (threadstr *) par;
    int rc1, rc2;
    int tmp;
    void *status;

    if(str->i < str->n){
        threadstr str1, str2;
        pthread_t tid1, tid2;

        str1.n = str2.n = str->n;
        str1.i = str2.i = str->i + 1;
        str1.tids = (long int*) malloc(str1.n*sizeof(long int));
        str2.tids = (long int*)malloc(str2.n*sizeof(long int));

        if(str1.tids == NULL || str2.tids == NULL){
            fprintf(stderr, "Memory allocation error.\n");
            pthread_exit((void *)-1);
        }

        for(tmp = 0; tmp<str->n; tmp++)
            str1.tids[tmp] = str2.tids[tmp] = str->tids[tmp];
        
        str1.tids[str1.i]=(long int) pthread_self();
        str2.tids[str2.i]=(long int) pthread_self();

        rc1 = pthread_create(&tid1, NULL, threadfunct, (void *) &str1);
        rc2 = pthread_create(&tid2, NULL, threadfunct, (void *) &str2);

        if(rc1 != 0 || rc2 != 0){
            fprintf(stderr, "Error on thread creation.\n");
            pthread_exit((void *) -1);
        }

        pthread_join(tid1, &status);
        pthread_join(tid2, &status);

        free(str1.tids);
        free(str2.tids);

        pthread_exit((void *)0);
    }
    else{

        for(tmp=0; tmp < str->n; tmp++)
            printf("%ld ", str->tids[tmp]);
        
        printf("\n");
        pthread_exit((void *)0);

    }
    
}