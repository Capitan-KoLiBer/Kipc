#include <stdio.h>
#include "kmsg.h"
#include "ksem.h"
#include "kshm.h"
int main() {
    printf("--------------------------------------------------\n");
    Kmsg *msg = kmsg_new(KMSG_KEY_RANDOM , NULL);
    msg = kmsg_create(msg , sizeof(char*));
    kmsg_send(msg , "SALAM : )");
    char *data_msg = kmsg_recv(msg);
    printf("Message queue read : %s\n",data_msg);
    msg = kmsg_destroy(msg);
    kmsg_free(msg);
    printf("--------------------------------------------------\n");
    Ksem *sem = ksem_new(KSEM_KEY_RANDOM , NULL);
    int sem_defaults[] = {1,2,3,4,5};
    sem = ksem_create(sem , 5 , sem_defaults);
    ksem_set(sem , KSEM_MODE_WAIT , 0 , -1);
    for(int a = 0 ; a < 5 ; a++){
        printf("Semaphore [%d] : %d\n",a , ksem_get(sem,a));
    }
    sem = ksem_destroy(sem);
    ksem_free(sem);
    printf("--------------------------------------------------\n");
    Kshm *shm = kshm_new(KSHM_KEY_RANDOM , NULL);
    shm = kshm_create(shm , KSHM_MODE_RDWR , KSHM_CONCURRENT_FULL , 1024);
    kshm_write(shm , 0 , "SALAM : ) ",1024);
    char *data_shm = kshm_read(shm , 0 , 1024);
    printf("Shared memory read : %s\n",data_shm);
    free(data_shm);
    kshm_set(shm , 0 , 1024 , 0);
    data_shm = kshm_read(shm , 0 , 1024);
    printf("Shared memory read : %s\n",data_shm);
    shm = kshm_destroy(shm);
    kshm_free(shm);
    printf("--------------------------------------------------\n");
    return 0;
}