#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void binary(int* vet, int n);

int main(int argc, char *argv[]){
    
    if(argc != 2){
        fprintf(stderr, "Wrong parameters number. Usage: %s n\n", argv[0]);
        return -1;
    }

    int n = atoi(argv[1]);
    int *vet=NULL;

    vet = (int*) malloc(n*sizeof(int));

    if(vet == NULL){
        fprintf(stderr, "Error in allocation phase.\n");
        return -1;
    }

    printf("Binary numbers of size %d:\n", n);
    binary(vet, n);
    free(vet);

    return 0;

}

void binary(int* vet, int n){
    int i;

    for(i=0; i<n; i++){
        if(fork()){
            vet[i]=1;
        }
        else
            vet[i]=0;
    }

    for(i=0; i<n; i++)
        printf("%d ", vet[i]);

    printf("\n");
    return;
}