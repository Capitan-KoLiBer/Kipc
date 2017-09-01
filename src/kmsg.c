#include "kmsg.h"

Kmsg *kmsg_new(KmsgKey kmsg_key , char *key_generator){
    Kmsg *kmsg = malloc(sizeof(Kmsg));
    switch (kmsg_key){
        case KMSG_KEY_PATH:
            kmsg->key = ftok(key_generator,1);
            break;
        case KMSG_KEY_RANDOM:
            kmsg->key = IPC_PRIVATE;
            break;
        case KMSG_KEY_CUSTOM:
            kmsg->key = (int) key_generator;
            break;
        default:
            kmsg->key = -1;
            break;
    }
    return kmsg;
}

Kmsg *kmsg_create(Kmsg *kmsg , size_t message_size){
    kmsg->id = msgget(kmsg->key , IPC_CREAT | 0660);
    kmsg->message_size = message_size;
    return kmsg;
}

void* kmsg_recv(Kmsg *kmsg){
    void *buffer = malloc(kmsg->message_size);
    if(msgrcv(kmsg->id , buffer , kmsg->message_size , 0 , 0) < 0){
        free(buffer);
    }
    return buffer;
}

void kmsg_send(Kmsg *kmsg , void *message){
    msgsnd(kmsg->id , message , kmsg->message_size , 0);
}

Kmsg *kmsg_destroy(Kmsg *kmsg){
    if(msgctl(kmsg->id , IPC_RMID , NULL) >= 0){
        kmsg->id = -1;
    }
    return kmsg;
}

void kmsg_free(Kmsg *kmsg){
    if(kmsg != NULL){
        free(kmsg);
    }
}