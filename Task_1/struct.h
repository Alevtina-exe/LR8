#pragma once
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <curl/curl.h>
#include <algorithm>

extern int size;
extern bool file, efile;

typedef struct quittance {
    char device[200];
    char serial_num[200];
    char name[200];
    struct {
        char str[200];
        int itgr;
    } date;
    union {
        bool bool_stat;
        char str_stat[25];
    } status;
} quittance;


