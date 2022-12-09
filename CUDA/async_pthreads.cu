#include <iostream>
#include <pthread.h>

#define cudaCheckErrors(msg) \
    do { \
        cudaError_t __err = cudaGetLastError(); \
        if (__err != cudaSuccess) { \
            fprintf(stderr, "Fatal error: %s (%s at %s:%d)\n", \
                msg, cudaGetErrorString(__err), \
                __FILE__, __LINE__); \
            fprintf(stderr, "*** FAILED - ABORTING\n"); \
            exit(1); \
        } \
    } while (0)


const int UNPROCESSED = 1;
const int PROCESSED = 2;
const int DONE = 3;
const int RUNNING= 0;
const int NUM_OF_DATA = 10;
const int NUM_OF_BLOCKS = 1;
const int THREADS_PER_BLOCK = 1;

//int data_states[NUM_OF_DATA];
cudaStream_t cuda_stream[2];
volatile int* process_state;
volatile int* d_process_state;
volatile int* d_copier_state;
int* d_data_state;
int* h_data_states;
int* h_copier_state;
cudaError_t cuda_status;

using namespace std;

void* copy_data(void* arg){
   int i=0;
   cudaSetDevice(0);
   cout << "in copy_data" << endl;
   while(i < NUM_OF_DATA){
      if (*process_state != UNPROCESSED){
        cout << "Now copy data " << i << " with state = " << h_data_states[i] <<  endl;
         *process_state = UNPROCESSED;
         cudaMemcpyAsync(d_data_state, &(h_data_states[i]), sizeof(int), cudaMemcpyHostToDevice, cuda_stream[0]);
         cudaStreamSynchronize(cuda_stream[0]);
         cudaCheckErrors("thread cudaMemcpyAsync fail");
         //*process_state = UNPROCESSED;
         i++;
         }
      }
   *h_copier_state = DONE;
   cudaMemcpyAsync((void *)d_copier_state, h_copier_state, sizeof(int), cudaMemcpyHostToDevice, cuda_stream[0]);
   cudaCheckErrors("thread cudaMemcpyAsync 2 fail");
  cout << "Thread finished" << endl;
   return NULL;
}

__global__ void process_data(volatile int* data_state, volatile int* process_state, volatile int* copier_state){
   int i = 0;
   printf("In process data: i = %d\n", i);
   while(*copier_state != DONE){
      printf(" i = %d, copier_state = %d, data_state = %d\n", i, *copier_state, *data_state);
      if(*data_state == UNPROCESSED){
        printf("now processing data %d\n", i);
        i++;
        // process data here, skipped
        *data_state = PROCESSED;
        __threadfence_system();
        *process_state = PROCESSED;
        __threadfence_system();
      }
   }
  printf("process_data is done\n");
}

int main(int argc, char **argv){
  int i;
  float elapsed_time;
  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start, 0);
  cudaEventRecord(stop, 0);

  cudaSetDevice(0);
  cudaSetDeviceFlags(cudaDeviceMapHost);
  cudaMallocHost((void**) &process_state, NUM_OF_BLOCKS*sizeof(int),  cudaHostAllocMapped);
  cudaCheckErrors("cudaMallocHost 1 fail");
  cudaHostGetDevicePointer((int**) &d_process_state, (int*) process_state, 0);
  cudaMalloc((void**) &d_copier_state, sizeof(int));
  cudaCheckErrors("cudaMalloc 1 fail");
  cudaMemset((void*)d_copier_state, RUNNING, sizeof(int));
  cudaMallocHost((void**) &h_copier_state, sizeof(int), 0);
  cudaCheckErrors("cudaMallocHost 3 fail");
  *h_copier_state = RUNNING;
  cudaMallocHost((void**) &h_data_states, NUM_OF_DATA*sizeof(int), 0);
  cudaCheckErrors("cudaMallocHost 2 fail");
  for(i = 0; i < NUM_OF_DATA; i++){
     h_data_states[i] = UNPROCESSED;
  }
  cudaMalloc((void**) &d_data_state, sizeof(int));
  cudaCheckErrors("cudaMalloc 2 fail");
  cudaMemcpy((void*)d_data_state, &(h_data_states[0]), sizeof(int), cudaMemcpyHostToDevice);

  cudaStreamCreate(&cuda_stream[0]);
  cudaStreamCreate(&cuda_stream[1]);
  pthread_t thread;
  int thread_state = pthread_create(&thread, NULL, &copy_data, NULL);
  if(thread_state){
     cout << "Error: unable to create thread (produce_instances), "<< thread_state << endl;
     exit(-1);
  }


  cout << "Starting kernel" << endl;
  process_data<<<NUM_OF_BLOCKS, THREADS_PER_BLOCK, 0, cuda_stream[1]>>>(d_data_state, d_process_state, d_copier_state);


  cudaDeviceSynchronize();

  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&elapsed_time, start, stop);
  cudaEventDestroy(start);
  cudaEventDestroy(stop);
  cout << "Elapsed time: " << elapsed_time << " ms" << endl;
  return 0;
}