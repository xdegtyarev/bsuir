/*
 ============================================================================
 Name        : md5.cu
 Author      : xdegtyarev
 Version     :
 Copyright   : alexander degtyarev
 Description : CUDA compute reciprocals
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <cuda.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>

static void CheckCudaErrorAux (const char *, unsigned, const char *, cudaError_t);
#define CUDA_CHECK_RETURN(value) CheckCudaErrorAux(__FILE__,__LINE__, #value, value)

// CUDA kernel that performs op on chunk of data
__global__ void baseChunkOpKernel(char* data, int chunkSize) {
	long threadSubchunkSize = chunkSize/blockDim.y;
	long threadStartOffset = threadSubchunkSize*threadIdx.y;
}

__global__ void printFileChunkOpKernel(char* data, int chunkSize) {
	long threadSubchunkSize = chunkSize/blockDim.y;
	long threadStartOffset = threadSubchunkSize*threadIdx.y;
	char* subbuf = (char*)malloc(threadSubchunkSize);
	for(int i = 0; i<threadSubchunkSize; i++){
		subbuf[i] = *(data+i+threadStartOffset);
	}
	printf("#################\n\n%d:[%s]\n\n##############",threadIdx.y,subbuf);
	free(subbuf);
}

__global__ void fileChunkCharCounterKernel(char* data, int chunkSize, unsigned long* res) {
	long threadSubchunkSize = chunkSize/blockDim.y;
	long threadStartOffset = threadSubchunkSize*threadIdx.y;
	char curr;
	for(int i = 0; i<threadSubchunkSize; i++){
		curr = *(data+i+threadStartOffset);
		res[curr]++;
	}
}

__global__ void fileChunkCompressorOpKernel(char* data, int chunkSize, char* res) {
	long threadSubchunkSize = chunkSize/blockDim.y;
	long threadStartOffset = threadSubchunkSize*threadIdx.y;
	char curr;
	char prev = 0;
	char acc = 0;
	long resC = threadStartOffset;
	for(int i = 0; i<threadSubchunkSize; i++){
		curr = *(data+i+threadStartOffset);
		if(curr == prev){
			acc++;
		}else{
			if(acc!=0){
				res[resC] = acc;
				resC++;
			}
			res[resC] = curr;
			acc = 0;
		}
	}
}

void printDeviceInfo(){
	int nDevices;
	cudaGetDeviceCount(&nDevices);
	for (int i = 0; i < nDevices; i++) {
		cudaDeviceProp prop;
		cudaGetDeviceProperties(&prop, i);
		printf(" Device %d: \"%s\" with Compute %d.%d capability\n",
				   i, prop.name, prop.major, prop.minor);
		printf("Total global memory:           %lu\n", prop.totalGlobalMem);
		printf("Number of multiprocessors:     %d\n", prop.multiProcessorCount);
		printf("Number of compute cores:     %d\n", prop.multiProcessorCount*192);
		printf("Total amount of shared memory per block: %lu\n",prop.sharedMemPerBlock);
		printf("Total registers per block:     %d\n", prop.regsPerBlock);
		printf("Warp size:                     %d\n", prop.warpSize);
		printf("Maximum memory pitch:          %lu\n", prop.memPitch);
		printf("Total amount of constant memory:         %lu\n",   prop.totalConstMem);
		printf("Memory Clock Rate (KHz): %d\n", prop.memoryClockRate);
		printf("Memory Bus Width (bits): %d\n", prop.memoryBusWidth);
		printf("Peak Memory Bandwidth (GB/s): %f\n\n",
			   2.0*prop.memoryClockRate*(prop.memoryBusWidth/8)/1.0e6);
	  }
}

void gpuBaseFileChunkRunner(char* data, size_t len, size_t chunkSz)
{
	size_t chunksCount = 512;
	size_t devBufferSize = chunkSz*chunksCount;
	printf("TotalBytes: %lu | Device BufferSize: %lu \n",len,devBufferSize);
	char* devBuf;
	CUDA_CHECK_RETURN(cudaMalloc(&devBuf, sizeof(char) * devBufferSize));

	size_t hostBytesLeft = len;
	size_t chunkSize;

	int numBlocks = 1;
	dim3 dimBlock(numBlocks,chunksCount);

	do
	{
		chunkSize = ( hostBytesLeft < devBufferSize) ? hostBytesLeft : devBufferSize;
		printf("Bytes left: %lu| total: %lu| reading: %lu\n",hostBytesLeft,len,chunkSize);
		CUDA_CHECK_RETURN(cudaMemcpy(devBuf, data, chunkSize * sizeof( char ) , cudaMemcpyHostToDevice));
		baseChunkOpKernel<<<numBlocks,dimBlock>>>(devBuf, chunkSize);
		CUDA_CHECK_RETURN(cudaDeviceSynchronize());
		data = data + chunkSize;
		hostBytesLeft = hostBytesLeft - chunkSize;
	} while( hostBytesLeft > 0 );
	CUDA_CHECK_RETURN(cudaFree(devBuf));
}

void gpuFileChunkRunner(char* data, size_t len, size_t chunkSz)
{
	size_t chunksCount = 512;
	size_t devBufferSize = chunkSz*chunksCount;
	printf("TotalBytes: %lu | Device BufferSize: %lu \n",len,devBufferSize);
	char* devBuf;
	unsigned long* res;
	unsigned long* result = (unsigned long*)malloc(sizeof(unsigned long) * CHAR_MAX);
	unsigned long* accresult = (unsigned long*)malloc(sizeof(unsigned long) * CHAR_MAX);
	for(int c = 0; c<CHAR_MAX; ++c){
		result[c] = 0;
		accresult[c] = 0;
	}

	CUDA_CHECK_RETURN(cudaMalloc(&devBuf, sizeof(char) * devBufferSize));
	CUDA_CHECK_RETURN(cudaMalloc(&res, sizeof(unsigned long) * CHAR_MAX));

	size_t hostBytesLeft = len;
	size_t chunkSize;

	int numBlocks = 1;
	dim3 dimBlock(numBlocks,chunksCount);

	do
	{
		chunkSize = ( hostBytesLeft < devBufferSize) ? hostBytesLeft : devBufferSize;
		printf("Bytes left: %lu| total: %lu| reading: %lu\n",hostBytesLeft,len,chunkSize);
		CUDA_CHECK_RETURN(cudaMemcpy(res,result,sizeof(unsigned long) * CHAR_MAX,cudaMemcpyHostToDevice));
		CUDA_CHECK_RETURN(cudaMemcpy(devBuf, data, chunkSize * sizeof( char ) , cudaMemcpyHostToDevice));
	    fileChunkCharCounterKernel<<<numBlocks,dimBlock>>>(devBuf, chunkSize,res);
	    CUDA_CHECK_RETURN(cudaDeviceSynchronize());
	    CUDA_CHECK_RETURN(cudaMemcpy( result, res, sizeof(unsigned long) * CHAR_MAX, cudaMemcpyDeviceToHost));

	    for(int c = 0; c<CHAR_MAX; ++c){
	    	accresult[c]=accresult[c]+result[c];
	    	result[c] = 0;
	    }
	    data = data + chunkSize;
	    hostBytesLeft = hostBytesLeft - chunkSize;
	} while( hostBytesLeft > 0 );

	printf("______\n");
	for(int c = 32; c<CHAR_MAX; ++c){
		printf("[%x] %c:%u\n",c,c,accresult[c]);
	}
	printf("______\n");
	CUDA_CHECK_RETURN(cudaFree(devBuf));
	CUDA_CHECK_RETURN(cudaFree(res));
	free(result);
	free(accresult);
}

int main(int argc, char *argv[])
{
	//check args
    if (argc < 2) {
    	fprintf(stderr, "%s\n", "File not specified");
        exit(EXIT_FAILURE);
    }else{
		bool debug_print = argc > 2;

		if(debug_print){
			printDeviceInfo();
		}

    	//check file
        if((ftok(argv[1],'R')) < 0){
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }

        struct stat64 st;
        stat64(argv[1], &st);
        long flen = st.st_size;
    	int fd;
    	if((fd = open(argv[1],O_RDONLY))<0){
    		perror("open");
    		exit(EXIT_FAILURE);
    	}
    	void* filep;
    	if((filep = mmap (0, flen, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED){
            perror("MMAP fail");
    	    exit(EXIT_FAILURE);
    	}

    	gpuBaseFileChunkRunner((char*)filep,flen,512);
//    	gpuFileChunkRunner((char*)filep,flen,512);

    	if ((close (fd)) < 0) {
			perror ("close");
			return 1;
    	}

    	if ((munmap (filep, flen)) < 0) {
			perror ("munmap");
			return 1;
		}

	    return 0;
    }
}

static void CheckCudaErrorAux (const char *file, unsigned line, const char *statement, cudaError_t err)
{
	if (err == cudaSuccess)
		return;
	printf("%s returned %s (err) %d at %s line: %d \n",statement,cudaGetErrorString(err),err,file,line);
	exit (1);
}

