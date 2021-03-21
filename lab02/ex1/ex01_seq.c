#include <stdlib.h>
#include <stdio.h>

void initRand(float *v, int n);

int main(int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "Wrong parameters number.\n");
        return 1;
    }

    int i, j;
    int n = atoi(argv[1]);
    float *v1, *v2, **mat;
    float *tmp, res=0;

    v1 = (float *)malloc(n*sizeof(float));
    v2 = (float *)malloc(n*sizeof(float));
    tmp= (float *)calloc(n, sizeof(float));

    mat = (float **)malloc(n*sizeof(float *));

    if(v1 == NULL || v2 == NULL || mat == NULL || tmp == NULL){
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
    
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            tmp[i] += v1[j]*mat[j][i];
        } 
    }

    printf("tmp vect is:\n");

    for(i=0; i<n; i++)
        printf("%f ", tmp[i]);
    
    printf("\n");
    printf("\n");

    for(i=0; i<n; i++){
        res += tmp[i]*v2[i];
    }

    printf("The result is: %f\n", res);

    free(v1);
    free(v2);

    for(i=0; i<n; i++)
        free(mat[i]);
    
    free(mat);

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