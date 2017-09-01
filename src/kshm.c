#include "kshm.h"

Kshm *kshm_new(KshmKey kshm_key , char *key_generator){
    Kshm *kshm = malloc(sizeof(Kshm));
    switch (kshm_key){
        case KSHM_KEY_PATH:
            kshm->key = ftok(key_generator,1);
            break;
        case KSHM_KEY_RANDOM:
            kshm->key = IPC_PRIVATE;
            break;
        case KSHM_KEY_CUSTOM:
            kshm->key = (int) key_generator;
            break;
        default:
            kshm->key = -1;
            break;
    }
    return kshm;
}

Kshm *kshm_create(Kshm *kshm , KshmMode kshm_mode , KshmConcurrent memory_concurrent , size_t memory_size){
    kshm->id = shmget(kshm->key , memory_size  , IPC_CREAT | 0660);
    if(kshm_mode == KSHM_MODE_RDWR){
        kshm->memory_address = shmat(kshm->id , (caddr_t*)0 , 0);
    }else {
        kshm->memory_address = shmat(kshm->id , (caddr_t*)0 , SHM_RDONLY);
    }
    kshm->memory_concurrent = memory_concurrent;
    if(memory_concurrent != KSHM_CONCURRENT_NONE){
        kshm->memory_semaphore = ksem_new(KSEM_KEY_CUSTOM , (char *) kshm->key);
        int ksem_defaults[] = {1};
        kshm->memory_semaphore = ksem_create(kshm->memory_semaphore , 1 , ksem_defaults);
    }
    return kshm;
}

void kshm_write(Kshm *kshm , int offset , void *data , int data_size){
    switch (kshm->memory_concurrent){
        case KSHM_CONCURRENT_FULL:
            ksem_set(kshm->memory_semaphore , KSEM_MODE_WAIT , 0 , -1);
            memcpy((kshm->memory_address+offset) , data , (size_t) data_size);
            ksem_set(kshm->memory_semaphore , KSEM_MODE_WAIT , 0 , 1);
            break;
        case KSHM_CONCURRENT_HALF:
            ksem_set(kshm->memory_semaphore , KSEM_MODE_WAIT , 0 , -1);
            memcpy((kshm->memory_address+offset) , data , (size_t) data_size);
            ksem_set(kshm->memory_semaphore , KSEM_MODE_WAIT , 0 , 1);
            break;
        default:
            memcpy((kshm->memory_address+offset) , data , (size_t) data_size);
            break;
    }
}

void kshm_set(Kshm *kshm , int offset , int length , int data){
    switch (kshm->memory_concurrent){
        case KSHM_CONCURRENT_FULL:
            ksem_set(kshm->memory_semaphore , KSEM_MODE_WAIT , 0 , -1);
            memset((kshm->memory_address+offset) , data , (size_t) length);
            ksem_set(kshm->memory_semaphore , KSEM_MODE_WAIT , 0 , 1);
            break;
        case KSHM_CONCURRENT_HALF:
            ksem_set(kshm->memory_semaphore , KSEM_MODE_WAIT , 0 , -1);
            memset((kshm->memory_address+offset) , data , (size_t) length);
            ksem_set(kshm->memory_semaphore , KSEM_MODE_WAIT , 0 , 1);
            break;
        default:
            memset((kshm->memory_address+offset) , data , (size_t) length);
            break;
    }
}

void* kshm_read(Kshm *kshm , int offset , int length){
    void *buffer = malloc(sizeof(size_t)*length);
    switch (kshm->memory_concurrent){
        case KSHM_CONCURRENT_FULL:
            ksem_wait(kshm->memory_semaphore , 0 , 1);
            memcpy(buffer , (kshm->memory_address+offset) , (size_t) length);
            break;
        case KSHM_CONCURRENT_HALF:
            memcpy(buffer , (kshm->memory_address+offset) , (size_t) length);
            break;
        default:
            memcpy(buffer , (kshm->memory_address+offset) , (size_t) length);
            break;
    }
    return buffer;
}

Kshm *kshm_destroy(Kshm *kshm){
    if(shmctl(kshm->id , 0 , IPC_RMID) >= 0){
        if(kshm->memory_semaphore != NULL){
            ksem_destroy(kshm->memory_semaphore);
        }
        kshm->id = -1;
    }
    return kshm;
}

void kshm_free(Kshm *kshm){
    if(kshm != NULL){
        ksem_free(kshm->memory_semaphore);
        free(kshm);
    }
}