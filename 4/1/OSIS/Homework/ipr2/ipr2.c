//
//  ipr2.c
//  OSIS
//
//  Created by Alexander Degtyarev on 10/31/16.
//  Copyright © 2016 Alexander Degtyarev. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <sys/stat.h>

#define BUFFER_SIZE 128
#define EXIT_FAILURE 1

void wait_sem(int* sem){
    while(*sem == 0){
        ;
    }
    *sem = 0;
}

void post_sem(int* sem){
    *sem = 1;
}

int main(int argc, char *argv[])
{
    //check args
    if (argc > 1) {
        key_t key;
        //check file
        if((key = ftok(argv[1],'R')) < 0){
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }
        struct stat st;
        stat(argv[1], &st);
        int flen = st.st_size;

        // creating shared seg
        int shmid;
        char *buffer;
        int* bytesRead;

        // if(shm_unlink(argv[1])<0){
        //     perror("Failed to get unlink");
        //     exit(EXIT_FAILURE);
        // }
        if((shmid = shm_open(argv[1],O_CREAT | O_RDWR, 0666)) < 0){
            perror("Failed to get shared mem");
            exit(EXIT_FAILURE);
        }
        if(ftruncate(shmid,BUFFER_SIZE) < 0){
            perror("Truncate error");
            exit(EXIT_FAILURE);
        }
        if((buffer = mmap(0,BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0)) == MAP_FAILED){
            perror("MMAP buffer");
            exit(EXIT_FAILURE);
        }

        if((bytesRead = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED,0,0)) == MAP_FAILED){
            perror("MMAP bytesRead");
            exit(EXIT_FAILURE);
        }
        //

        // created pseudo semaphores as default don't work well on macOS
        char* fullsem;
        char* emptysem;
        //creating semaphore in shared mem
        fullsem = mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED,0,0);
        if(fullsem == MAP_FAILED){
            perror("Unable to create shared full semaphore");
            exit(EXIT_FAILURE);
        }
        *fullsem = 0;

        emptysem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED,0,0);
        if(emptysem == MAP_FAILED){
            perror("Unable to create shared empty semaphore");
            exit(EXIT_FAILURE);
        }
        *emptysem = 1;

        printf("%s %d %s %d\n", "full is", *fullsem, "empty is", *emptysem);
        // sem_t *full;
        // sem_t *empty;
        // if((full = sem_open("/full",  O_CREAT, 0644 , 0)) < 0){
        //     perror("Unable to create full semaphore");
        //     sem_unlink("/full");
        //     exit(EXIT_FAILURE);
        // }
        // if((empty = sem_open("/empty",  O_CREAT, 0644 , 1)) < 0){
        //     perror("Unable to create empty semaphore");
        //     sem_unlink("/empty");
        //     exit(EXIT_FAILURE);
        // }
        //

        //Start copying
        printf("%s\n", "Start Copying");
        pid_t pid = fork();
        if(pid < 0){
            perror("Failed on fork");
        }
        else if(pid == 0){
            printf("%s %d\n", "Writer", (int) getpid());

            char *addName = "(copy)";
            char *dstFileName = malloc(strlen(argv[1]) + strlen(addName) + 1);
            strcpy(dstFileName,argv[1]);
            strcat(dstFileName, addName);

            //open write file
            FILE *wfp = fopen(dstFileName,"wb");
            if(!wfp){
                perror(dstFileName);
                exit(EXIT_FAILURE);
            }

            int totalBytesWrite = 0;

            while(totalBytesWrite < flen){
                wait_sem(fullsem);// sem_wait(full);
                totalBytesWrite += fwrite(buffer,1,*bytesRead, wfp);
                printf("%s %d \n", "w: ", totalBytesWrite);
                post_sem(emptysem);// sem_post(empty);
            }

            printf("%s\n", "File writing - Done");
            fclose(wfp);
            free(dstFileName);
            return 0;
        }else{
            printf("%s %d\n", "Reader", (int) getpid());
            //readbuffer is private to process
            char *readbuffer = malloc(BUFFER_SIZE);
            //open read file
            FILE *rfp = fopen(argv[1],"rb");
            if(!rfp){
                perror(argv[1]);
                exit(EXIT_FAILURE);
            }
            int totalBytesRead = 0;
            while(totalBytesRead < flen){
                *bytesRead = fread(readbuffer, 1, BUFFER_SIZE, rfp);
                if (*bytesRead < BUFFER_SIZE && ferror(rfp) ){
                    perror("Error reading file");
                    exit(EXIT_FAILURE);
                }
                totalBytesRead += (*bytesRead);
                printf("%s %d \n", "r: ", totalBytesRead);
                wait_sem(emptysem);// sem_wait(empty);
                memcpy(buffer,readbuffer,BUFFER_SIZE);
                post_sem(fullsem);// sem_post(full);
            }

            printf("\n%s\n", "File reading - Done");
            fclose(rfp);
            free(readbuffer);
            if(shm_unlink(argv[1])<0){
                perror("Failed to exit unlink");
                exit(EXIT_FAILURE);
            }
            printf("%s\n", "Done Copying");
            return 0;
        }
    } else {
        fprintf(stderr, "%s\n", "File not specified");
        exit(EXIT_FAILURE);
    }
    return 0;
}