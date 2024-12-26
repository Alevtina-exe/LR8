#pragma once
#include <fstream>
#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include <algorithm>

extern int size;
extern bool file, efile;

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


