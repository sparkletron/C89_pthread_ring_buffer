/* ring buffer test */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>

#include "ringBuffer.h"

/* 8 MB */
#define BUFFSIZE  (1 << 23)
/* 1 MB */
#define DATACHUNK (1 << 20)

struct s_ringBuffer *p_ringBuffer = NULL;

void *producer(void *data);
void *consumer(void *data);

int main(int argc, char *argv[])
{
  int error = 0;
  int opt   = 0;
  
  pthread_t producerThread;
  pthread_t consumerThread;
  
  FILE *p_inFile = NULL;
  FILE *p_outFile = NULL;

  char inFileName[256]  = "input.txt";
  char outFileName[256] = "output.txt";

  while((opt = getopt(argc, argv, "i:o:h")) != -1)
  {
    switch(opt)
    {
      case 'i':
        strcpy(inFileName, optarg);
        break;
      case 'o':
        strcpy(outFileName, optarg);
        break;
      default:
        printf("Usage: %s -i filein.txt -o fileout.txt\n", argv[0]);
        return EXIT_SUCCESS;
    }
  }
  
  p_inFile = fopen(inFileName, "r");
  
  if(!p_inFile)
  {
    perror("File IO Issue.");
    
    return EXIT_FAILURE;
  }
  
  p_outFile = fopen(outFileName, "w");
  
  if(!p_outFile)
  {
    perror("File IO Issue.");
    
    return EXIT_FAILURE;
  }
  
#ifdef DEBUG_STATUS
  printf("CREATING RING BUFFER\n");
#endif
  
  p_ringBuffer = initRingBuffer(BUFFSIZE, 1);
  
  if(!p_ringBuffer)
  {
    fprintf(stderr, "Failed to create ring buffer.\n");
    
    fclose(p_outFile);
    fclose(p_inFile);
    
    return EXIT_FAILURE;
  }

#ifdef DEBUG_STATUS
  printf("CREATING PRODUCER THREAD\n");
#endif

  error = pthread_create(&producerThread, NULL, producer, p_inFile);
  
  if(error)
  {
    fprintf(stderr, "Failed to create producer thread.\n");
    
    fclose(p_outFile);
    fclose(p_inFile);
    
    freeRingBuffer(&p_ringBuffer);
    
    return EXIT_FAILURE;
  }
  
#ifdef DEBUG_STATUS
  printf("CREATING CONSUMER THREAD\n");
#endif

  error = pthread_create(&consumerThread, NULL, consumer, p_outFile);
  
  if(error)
  {
    fprintf(stderr, "Failed to create consumer thread.\n");
    
    fclose(p_outFile);
    fclose(p_inFile);
    
    ringBufferEndBlocking(p_ringBuffer);
    
    pthread_join(producerThread, NULL);
    
    freeRingBuffer(&p_ringBuffer);
    
    return EXIT_FAILURE;
  }

#ifdef DEBUG_STATUS
  printf("THREAD CREATED, WAITING FOR PRODUCER\n");
#endif
  
  pthread_join(producerThread, NULL);

#ifdef DEBUG_STATUS
  printf("PRODUCER JOINED, WAITING FOR CONSUMER.\n");
#endif
  
  pthread_join(consumerThread, NULL);

#ifdef DEBUG_STATUS
  printf("CONSUMER JOINED, ENDING PROGRAM.\n");
#endif

  freeRingBuffer(&p_ringBuffer);
  
  fclose(p_inFile);
  fclose(p_outFile);
  
  return EXIT_SUCCESS;
}

void *producer(void *data)
{
  char *p_fileBuffer = NULL;
  
  FILE *p_inFile = NULL;
  
  p_inFile = (FILE *)data;
  
  if(!p_inFile)
  {
    fprintf(stderr, "File discriptor is NULL.\n");
    return NULL;
  }
  
  p_fileBuffer = malloc(DATACHUNK);
  
  if(!p_fileBuffer)
  {
    perror("Could not allocate producer buffer.");
    return NULL;
  }
  
  do
  {
    int numElemRead = 0;
    int numElemWrote = 0;
    
    numElemRead = fread(p_fileBuffer, sizeof(*p_fileBuffer), DATACHUNK, p_inFile);

    do
    {
      numElemWrote += ringBufferBlockingWrite(p_ringBuffer, p_fileBuffer + numElemWrote, numElemRead - numElemWrote, NULL);
    } while(numElemWrote < numElemRead);
    
  } while(!feof(p_inFile));
  
  ringBufferEndBlocking(p_ringBuffer);
  
  free(p_fileBuffer);
  
  return NULL;
}

void *consumer(void *data)
{
  char *p_fileBuffer = NULL;
  
  FILE *p_outFile = NULL;
  
  p_outFile = (FILE *)data;
  
  if(!p_outFile)
  {
    fprintf(stderr, "File discriptor is NULL.\n");
    return NULL;
  }
  
  p_fileBuffer = malloc(DATACHUNK);
  
  if(!p_fileBuffer)
  {
    perror("Could not allocate producer buffer.");
    return NULL;
  }
  
  do
  {
    int numElemRead = 0;
    int numElemWrote = 0;
    
    numElemRead = ringBufferBlockingRead(p_ringBuffer, p_fileBuffer, DATACHUNK, NULL);

    do
    {
      numElemWrote += fwrite(p_fileBuffer + numElemWrote, sizeof(*p_fileBuffer), numElemRead - numElemWrote, p_outFile);
    } while(numElemRead < numElemWrote);

  } while(ringBufferIsAlive(p_ringBuffer));
  
  free(p_fileBuffer);
  
  return NULL;
}
