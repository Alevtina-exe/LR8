#pragma once

typedef struct queue {
    char surname[200];
    char name[200];
    char midname[200];
    char date[200];
    union {
        long long itgr;
        char str[200];
    } num;
    char address[200];
} queue;


