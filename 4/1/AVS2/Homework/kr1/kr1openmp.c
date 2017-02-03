//
//  kr1openmp.c
//  AVS
//
//  Created by Alexander Degtyarev on 10/31/16.
//  Copyright © 2016 Alexander Degtyarev. All rights reserved.
//
// gcc-6 -fopenmp -o kr1 kr1openmp.c

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, const char * argv[]) {
    bool debug_info = argc > 1;
    const float TOP_CELL_VAL = 10.0;
    unsigned int rows = 0;
    unsigned int columns = 0;
    char* readbuf = malloc(1024*sizeof(char));

    //synchronized data input
    do{
        printf("Set row count\n");
        if(scanf("%4u",&rows) > 0 && rows > 0 && rows < 10000){
            break;
        }else{
            fgets(readbuf,1024,stdin);
            printf("Set val in range [1,9999]\n");
        }
    }while(true);

    do{
        printf("Set collumn count\n");
        if(scanf("%4u",&columns) > 0 && columns > 0 && columns < 10000){
            break;
        }else{
            fgets(readbuf,1024,stdin);
            printf("Set val in range [1,9999]\n");
        }
    }while(true);

    //allocating resources
    float *vector = malloc (rows * sizeof(float));
    float *result = malloc (columns * sizeof(float));
    float **matrix;
    matrix = (float **) malloc(rows*sizeof(float *));
    for(int i = 0; i < rows; i++){
        matrix[i]=(float *) malloc(columns * sizeof(float));
    }

    printf("\n -SRAND-\n");
    //parallel randomize matrix
    #pragma omp parallel shared(rows,columns,matrix,vector)
    {
        srand((unsigned int)time(NULL) ^ omp_get_thread_num());
    }

    printf("\n -RANDOMIZER-\n");
    #pragma omp parallel for shared(rows,columns,matrix,vector,result)
    for(int p_idx = 0; p_idx < rows*columns; ++p_idx)
    {
        int p_i = p_idx/columns;
        int p_j = p_idx%columns;
        if(debug_info) printf("befor pidx:i:j %d:%d:%d thread: %d:%d\n",p_idx,p_i,p_j,omp_get_thread_num(), omp_get_num_threads());

        result[p_i] = 0.0;
        if(p_i==0){
            vector[p_j] = ((float)rand()/(float)(RAND_MAX)) * TOP_CELL_VAL;
        }
        matrix[p_i][p_j] = ((float)rand()/(float)(RAND_MAX)) * TOP_CELL_VAL;

        if(debug_info) printf("after pidx:i:j %d:%d:%d thread: %d:%d\n",p_idx,p_i,p_j,omp_get_thread_num(), omp_get_num_threads());
    }

    //synchronized print
    printf("\n -MATRIX-\n");

    for(int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            printf(" %3.2f",matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n -VECTOR-\n");

    for (int j = 0; j < columns; ++j)
    {
        printf(" %3.2f\n",vector[j]);
    }

    printf("\n -----------------\n");
    //parallel multiplication
    #pragma omp parallel for shared(rows,columns,matrix,vector,result)
        for(int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < columns; ++j)
            {
                result[i]+=matrix[i][j]*vector[j];
                if(debug_info) printf("Thread: %d:%d\n",omp_get_thread_num(), omp_get_num_threads());
            }
        }

    //synchronized output
    printf("\n -RESULT-\n");
    for(int i = 0; i < rows; ++i)
    {
        printf(" %2.1f \n",result[i]);
    }

    free(vector);
    free(result);
    free(matrix);

    return 0;
}