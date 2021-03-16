#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LEN 30
#define ROW_LEN 78

typedef struct mystrct{
    int id;
    long int reg;
    char surname[MAX_LEN+1];
    char name[MAX_LEN+1];
    int mark;
}mystruct;

int main(int argc, char *argv[]){

    if(argc != 4){
        fprintf(stderr, "Wrong parameter number.\n");
        exit(1);
    }

    FILE *inFile = fopen(argv[1], "r");
    int outBin = open(argv[2], O_RDWR | O_CREAT | O_TRUNC | O_SYNC, S_IRWXU);
    FILE *outFrm = fopen(argv[3], "w");

    if(inFile == NULL || outBin == -1 || outFrm == NULL){
        fprintf(stderr, "Error in opening files.\n");
        exit(1);
    }

    mystruct buffer;
    int nR;
    char row[ROW_LEN+1];

    while(fscanf(inFile, "%d %ld %s %s %d", &buffer.id, &buffer.reg, buffer.surname, buffer.name, &buffer.mark) != EOF){

        nR = write(outBin, &buffer.id, sizeof(buffer.id));
        nR += write(outBin, &buffer.reg, sizeof(buffer.reg));
        nR += write(outBin, buffer.surname, sizeof(buffer.surname));
        nR += write(outBin, buffer.name, sizeof(buffer.name));
        nR += write(outBin, &buffer.mark, sizeof(buffer.mark));

        if(nR != ROW_LEN){
            fprintf(stderr, "Write error: %d", nR);
        }
        
    }

    lseek(outBin, 0, SEEK_SET);

    while((nR = read(outBin, &buffer.id, sizeof(int))) > 0 ){
        
        read(outBin, &buffer.reg, sizeof(long int));
        read(outBin, buffer.surname, 31*sizeof(char));
        read(outBin, buffer.name, 31*sizeof(char));
        read(outBin, &buffer.mark, sizeof(int));
        
        fprintf(outFrm, "%d %ld %s %s %d\n", buffer.id, buffer.reg, buffer.surname, buffer.name, buffer.mark);
    }

    fclose(inFile);
    close(outBin);
    fclose(outFrm);

    return 0;
}