#include "ksem.h"
#include "stdio.h"
Ksem *ksem_new(KsemKey ksem_key , char *key_generator){
    Ksem *ksem = malloc(sizeof(Ksem));
    switch (ksem_key){
        case KSEM_KEY_PATH:
            ksem->key = ftok(key_generator,1);
            break;
        case KSEM_KEY_RANDOM:
            ksem->key = IPC_PRIVATE;
            break;
        case KSEM_KEY_CUSTOM:
            ksem->key = (int) key_generator;
            break;
        default:
            ksem->key = -1;
            break;
    }
    return ksem;
}

Ksem *ksem_create(Ksem *ksem , int sem_count , int sem_defaults[]){
    ksem->id = semget(ksem->key , sem_count , IPC_CREAT | 0660);
    while(sem_count--){
        semctl(ksem->id , sem_count , SETVAL , sem_defaults[sem_count]);
    }
    return ksem;
}

int ksem_get(Ksem *ksem , int sem_num){
    return semctl(ksem->id , sem_num , GETVAL);
}

void ksem_wait(Ksem *ksem , int sem_num , int min_value){
    ksem_set(ksem , KSEM_MODE_WAIT , sem_num , -min_value);
    ksem_set(ksem , KSEM_MODE_WAIT , sem_num , min_value);
}

int ksem_set(Ksem *ksem , KsemMode sem_mode , int sem_num , int value){
    struct sembuf buffer;
    buffer.sem_num = (unsigned short) sem_num;
    buffer.sem_op = (short) value;
    if(sem_mode == KSEM_MODE_NOWAIT){
        buffer.sem_flg = IPC_NOWAIT;
    }else{
        buffer.sem_flg = SEM_UNDO;
    }
    return semop(ksem->id , &buffer , 1);
}

Ksem *ksem_destroy(Ksem *ksem){
    if(semctl(ksem->id , 0 , IPC_RMID) >= 0){
        ksem->id = -1;
    }
    return ksem;
}

void ksem_free(Ksem *ksem){
    if(ksem != NULL){
        free(ksem);
    }
}