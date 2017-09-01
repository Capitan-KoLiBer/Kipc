#ifndef KIPC_KSHM_H
#define KIPC_KSHM_H

/// Simple and powerful implementation of System V Shared Memories !

#include "memory.h"
#include "sys/shm.h"
#include "stdlib.h"
#include "ksem.h"

typedef enum KshmKey{
    KSHM_KEY_PATH,
    KSHM_KEY_RANDOM,
    KSHM_KEY_CUSTOM
} KshmKey;

typedef enum KshmMode{
    KSHM_MODE_RD,
    KSHM_MODE_RDWR
} KshmMode;

typedef enum KshmConcurrent{
    KSHM_CONCURRENT_FULL,   // when writing     -> no reading   ,   no writing
    KSHM_CONCURRENT_HALF,   // when writing     -> reading      ,   no writing
    KSHM_CONCURRENT_NONE    // when writing     -> reading      ,   writing
} KshmConcurrent;

typedef struct Kshm{
    int key;
    int id;
    void *memory_address;
    KshmConcurrent  memory_concurrent;
    Ksem *memory_semaphore;
} Kshm;

Kshm *kshm_new(KshmKey kshm_key , char *key_generator);

Kshm *kshm_create(Kshm *kshm , KshmMode kshm_mode , KshmConcurrent memory_concurrent , size_t memory_size);

void kshm_write(Kshm *kshm , int offset , void *data , int data_size);

void kshm_set(Kshm *kshm , int offset , int length , int data);

void* kshm_read(Kshm *kshm , int offset , int length);

Kshm *kshm_destroy(Kshm *kshm);

void kshm_free(Kshm *kshm);

#endif //KIPC_KSHM_H
