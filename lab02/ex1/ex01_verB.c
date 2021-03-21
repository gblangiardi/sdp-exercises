#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void initRand(float *v, int n);
void *threadF(void *par);

typedef struct tstr{
    int i, n;
    float *v1, *v2, *v;
    float **mat;
}t_struct;

int count=0;
pthread_mutex_t *mEx;


int main(int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "Wrong parameters number.\n");
        return 1;
    }

    int i, j;
    int n = atoi(argv[1]);
    t_struct *mystruct;
    pthread_t *tids;
    float *v1, *v2, *v, **mat;
    float res =0;

    mystruct = (t_struct *)malloc(n*sizeof(t_struct));
    v1 = (float *)malloc(n*sizeof(float));
    v2 = (float *)malloc(n*sizeof(float));
    v = (float *)calloc(n, sizeof(float));
    tids = (pthread_t *)malloc(n*sizeof(pthread_t));
    mEx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

    mat = (float **)malloc(n*sizeof(float *));

    if(v1 == NULL || v2 == NULL || mat == NULL || tids == NULL || v == NULL || mEx == NULL){
        fprintf(stderr, "Error in memory alllocation phase 1.\n");
        return 1;
    }

    for(i=0; i<n; i++){
        mat[i]=(float *)malloc(n*sizeof(float));
        if(mat[i] == NULL){
            fprintf(stderr, "Error in memory alllocation phase 2.\n");
            return 1;
        }

        initRand(mat[i], n);
    }

    printf("\n");

    initRand(v1, n);
    printf("\n");
    initRand(v2, n);
    printf("\n");

    pthread_mutex_init(mEx, NULL);

    for(i=0; i<n; i++){
        mystruct[i].i=i;
        mystruct[i].n=n;
        mystruct[i].v=v;
        mystruct[i].v1=v1;
        mystruct[i].v2=v2;
        mystruct[i].mat=mat;
        pthread_create(&tids[i], NULL, threadF, (void *)&mystruct[i]);
    }

    for(i=0; i<n; i++){
        pthread_join(tids[i], NULL);
    }

    free(v1);
    free(v2);
    free(v);
    free(tids);

    for(i=0; i<n; i++)
        free(mat[i]);
    
    free(mat);

    pthread_mutex_destroy(mEx);

    return 0;
}

void initRand(float *v, int n){
    int i;
    for(i=0; i<n; i++)
        v[i]= ((float)rand()/RAND_MAX) - 0.5;
    
    for(i=0; i<n; i++)
        printf("%f ", v[i]);
    
    printf("\n");

    return;
}

void *threadF(void *par){
    t_struct *str = (t_struct *)par;
    int j, i;


    for(j=0; j<str->n; j++){
        str->v[str->i] += str->v1[j]*str->mat[j][str->i];
    }

    pthread_mutex_lock(mEx);

    if(count < str->n-1){
        count++;
        pthread_mutex_unlock(mEx);
        pthread_exit(NULL);
    }
    else if(count == str->n -1){
        count++;
        printf("Last thread to terminate phase 1 was thread number %d\n", str->i+1);
        pthread_mutex_unlock(mEx);

        printf("tmp v vector is:\n");

        for(i=0; i<str->n; i++)
            printf("%f ", str->v[i]);
        
        printf("\n");
        printf("\n");

        float res=0;

        for(i=0; i<str->n; i++){
            res += str->v[i]*str->v2[i];
        }

        printf("The result is: %f\n", res);
    }

    pthread_exit(NULL);
}