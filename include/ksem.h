#ifndef KIPC_KSEM_H
#define KIPC_KSEM_H

/// Simple and powerful implementation of System V Semaphores !

#include "sys/sem.h"
#include "stdlib.h"

typedef struct Ksem{
    int key;
    int id;
} Ksem;

typedef enum KsemKey{
    KSEM_KEY_PATH,
    KSEM_KEY_RANDOM,
    KSEM_KEY_CUSTOM
} KsemKey;

typedef enum KsemMode{
    KSEM_MODE_WAIT,
    KSEM_MODE_NOWAIT
} KsemMode;

Ksem *ksem_new(KsemKey ksem_key , char *key_generator);

Ksem *ksem_create(Ksem *ksem , int sem_count , int sem_defaults[]);

int ksem_get(Ksem *ksem , int sem_num);

void ksem_wait(Ksem *ksem , int sem_num , int min_value);

int ksem_set(Ksem *ksem , KsemMode sem_mode , int sem_num , int value);

Ksem *ksem_destroy(Ksem *ksem);

void ksem_free(Ksem *ksem);

#endif //KIPC_KSEM_H
