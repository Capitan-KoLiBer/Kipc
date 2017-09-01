#ifndef KIPC_KMSG_H
#define KIPC_KMSG_H

/// Simple and powerful implementation of System V Message Queues !

#include "sys/msg.h"
#include "stdlib.h"

typedef struct Kmsg{
    int key;
    int id;
    size_t message_size;
} Kmsg;

typedef enum KmsgKey{
    KMSG_KEY_PATH,
    KMSG_KEY_RANDOM,
    KMSG_KEY_CUSTOM
} KmsgKey;

Kmsg *kmsg_new(KmsgKey kmsg_key , char *key_generator);

Kmsg *kmsg_create(Kmsg *kmsg , size_t message_size);

void* kmsg_recv(Kmsg *kmsg);

void kmsg_send(Kmsg *kmsg , void *message);

Kmsg *kmsg_destroy(Kmsg *kmsg);

void kmsg_free(Kmsg *kmsg);

#endif //KIPC_KMSG_H
