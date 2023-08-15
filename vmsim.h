#ifndef VMSIM_H
#define VMSIM_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


#define TRACE_1 16
#define TRACE_2 64
#define PAGE_COUNT 32
#define FRAME_SIZE 2048
#define MAX_FRAME_SIZE 65546
#define BIG_NUM 10000
#define SMOL_NUM -1


struct memory_referance{
    char adress[6];
    int to_page;
};

struct page_table{
    int v_bit;
};

struct free_frame_list{
    int page_number;
    int page_time_in;
    int last_time_referanced;
};


#endif
