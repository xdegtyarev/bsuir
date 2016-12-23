//
//  kr1openmp.c
//  AVS
//
//  Created by Alexander Degtyarev on 10/31/16.
//  Copyright © 2016 Alexander Degtyarev. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, const char * argv[]) {
    const float TOP_CELL_VAL = 10.0;
    unsigned int rows = 0;
    unsigned int columns = 0;

    //synchronized data input
    printf("Set row count\n");
    scanf("%u",&rows);
    printf("Set collumn count\n");
    scanf("%u",&columns);

    //allocating resources
    float *vector = malloc (rows * sizeof(float));
    float *result = malloc (columns * sizeof(float));
    float **matrix;
    matrix = (float **) malloc(rows*sizeof(float *));
    for(int i = 0; i < rows; i++){
        matrix[i]=(float *) malloc(columns * sizeof(float));
    }

    //parallel randomize matrix
    #pragma omp parallel shared(rows,columns,matrix,vector)
    {
        srand((unsigned int)time(NULL) ^ omp_get_thread_num());
        #pragma omp for
        for(int i = 0; i < rows; ++i)
        {
            result[i] = 0.0;
            for (int j = 0; j < columns; ++j)
            {
                if(i==0){
                    vector[j] = ((float)rand()/(float)(RAND_MAX)) * TOP_CELL_VAL;
                }
                matrix[i][j] = ((float)rand()/(float)(RAND_MAX)) * TOP_CELL_VAL;
                // printf(" %d.%d Thread: %d:%d\n",i,j,omp_get_thread_num(), omp_get_num_threads());
            }
        }
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

    // printf("\n -----------------\n");
    //parallel multiplication
    #pragma omp parallel for shared(rows,columns,matrix,vector,result)
        for(int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < columns; ++j)
            {
                result[i]+=matrix[i][j]*vector[j];
                // printf("Thread: %d:%d\n",omp_get_thread_num(), omp_get_num_threads());
            }
        }

    //synchronized output
    printf("\n -RESULT-\n");
    for(int i = 0; i < rows; ++i)
    {
        printf(" %2.1f \n",result[i]);
    }

    return 0;
}