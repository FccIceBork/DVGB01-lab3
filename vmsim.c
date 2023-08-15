#include "vmsim.h"
#include "help.h"


//init help functions
void hex_to_dec(struct memory_referance *hex, int size);
int check_ram(struct page_table *page_t, struct free_frame_list *ram, int size, int page, int time);
int free_space(struct page_table *page_t, struct free_frame_list *ram, int size, int page, int time);
void page_replacement(struct memory_referance *m_referance, struct page_table *page_t, struct free_frame_list *ram, int ram_size, int a_size, int page, int time, char *alg);


//sim
int main(int argc, char *argv[])
{
    //Init
    char *algo;
    char *sorce;
    int frames;
    //int memory_accesses;
    int page_hit;
    int page_fault;
    int page_replacements;
    int adress_space = TRACE_2;
    int counter;

    //Read arguments
    if(argc < 4){
        printf("To few arguments!\n");
        return 0;
    }else if(argc > 4){
        printf("To many arguments!\n");
        return 0;
    }

    frames = atoi(argv[1]);
    if(frames == 0){
        printf("You need to have more than zero frames to run this sim!\n!!!SIM EXICUTION FAILED!!!\n");
        return 0;
    }

    if(strcmp(argv[2], "FIFO") == 0 || strcmp(argv[2], "Optimal") == 0 || strcmp(argv[2], "LRU") == 0){
        algo = argv[2];
    }else{
        printf("Worng algorithm!\n");
        return 0;
    }

    if(strcmp(argv[3], "lab3/trace_1.dat") == 0 || strcmp(argv[3], "lab3/trace_2.dat") == 0){
        sorce = argv[3];
    }else{
        printf("Worng trace file!\n");
        return 0;
    }

    //Read memory referancees
    if(strcmp(sorce, "lab3/trace_1.dat") == 0){
        adress_space = TRACE_1;
    }
    struct memory_referance mr[adress_space];

    FILE* tf;
    tf = fopen(sorce, "r");
    if(tf == NULL){
        printf("File can't open!\n");
    }

    counter = 0;
    while(!feof(tf)){
        fscanf(tf, "%s", mr[counter].adress);
        counter++;
    }

    fclose(tf);

    //Initialize virtual memory
    hex_to_dec(mr, adress_space);

    struct page_table pt[PAGE_COUNT];
    for(int i=0;i<PAGE_COUNT;i++){
        pt[i].v_bit = 0;
    }

    struct free_frame_list ffl[frames];
    for(int i=0;i<frames;i++){
        ffl[i].page_number = -1;
        ffl[i].page_time_in = -1;
        ffl[i].last_time_referanced = -1;
    }

    page_fault = 0;
    page_hit = 0;
    page_replacements = 0;

    //Loop
    for(int i=0;i<adress_space;i++){
        printf("Memory access for %s\n", mr[i].adress);
        /*Check if the page is in ram*/
        if(check_ram(pt, ffl, frames, mr[i].to_page, i)){
            page_hit++;
            printf("\033[1;32mPage hit\033[0m\n");
        }else{
            if(free_space(pt, ffl, frames, mr[i].to_page, i)){
                page_fault++;
                printf("\033[1;31mPage fault\033[0m\n");
            }else{
                page_replacement(mr, pt, ffl, frames, adress_space, mr[i].to_page, i, algo);
                page_replacements++;
                page_fault++;
                printf("\033[1;31mPage fault\033[0m\n");
                printf("\033[1;33mPage replacement\033[0m\n");
            }
        }
    }


    //Display results
    for(int i=0;i<20;i++){
        printf("_");
    }
    printf("\nNumbers of frames: %d\nNumbers of memory access: %d\n\nPage hits: %d\nPage faults: %d\nPage replacements: %d\n",
     frames, adress_space, page_hit, page_fault, page_replacements);
    for(int i=0;i<20;i++){
        printf("_");
    }
    printf("\nSim end\n");
    //printf("You got to the end! ;)\n");
    return 0;
}


//help functions
void hex_to_dec(struct memory_referance *hex, int size){
    int dec, temp, len = 6;
    for(int i=0; i<size; i++){
        temp = 0;
        dec = 0;
        /*Starting after 0x to find the dec of the hex*/
        for(int j=2; j<len; j++){
            /*Finding the decimal rep of the hex*/
            if(hex[i].adress[j]>='0' && hex[i].adress[j]<='9'){
                temp = hex[i].adress[j] - 48;
            }else if(hex[i].adress[j]>='A' && hex[i].adress[j]<='F'){
                temp = hex[i].adress[j] - 65 + 10;
            }
            /*To use pow, when compiling add -lm to link with math lib*/
            dec += temp * pow(16, len - j - 1);
        }
        hex[i].to_page = dec / FRAME_SIZE;
    }
}
int check_ram(struct page_table *page_t, struct free_frame_list *ram, int size, int page, int time){
    if(page_t[page].v_bit == 1){
        for(int i=0;i<size;i++){
            if(ram[i].page_number == page){
                ram[i].last_time_referanced = time;
                return 1;
            }
        }
    }
    return 0;
}
int free_space(struct page_table *p_table, struct free_frame_list *ram, int size, int page, int time){
    for(int i=0;i<size;i++){
        if(ram[i].page_number == -1){
            ram[i].page_number = page;
            ram[i].page_time_in = time;
            ram[i].last_time_referanced = time;
            p_table[page].v_bit = 1;
            return 1;
        }
    }
    return 0;
}
void page_replacement(struct memory_referance *m_referance, struct page_table *page_t, struct free_frame_list *ram, int ram_size, int a_size, int page, int time, char *alg){
    int temp, place, tp, replacmets, mr_place, opt_temp;
    replacmets = 0;
    temp = BIG_NUM;
    opt_temp = SMOL_NUM;
    if(strcmp(alg, "FIFO") == 0){
        for(int i=0;i<ram_size;i++){
            if(temp > ram[i].page_time_in){
                temp = ram[i].page_time_in;
                tp = ram[i].page_number;
                place = i;
                replacmets = 1;
            }
        }
    }else if(strcmp(alg, "LRU") == 0){
        for(int i=0;i<ram_size;i++){
            if(temp > ram[i].last_time_referanced){
                temp = ram[i].last_time_referanced;
                tp = ram[i].page_number;
                place = i;
                replacmets = 1;
            }
        }
    }else if(strcmp(alg, "Optimal") == 0){
        opt_temp = 0;
        for(int i=0;i<ram_size;i++){
            temp = ram[i].page_number;
            for(mr_place=time;mr_place<a_size;mr_place++){
                if(m_referance[mr_place].to_page == temp){
                    break;
                }
            }
            if((mr_place - time) > opt_temp){
                tp = temp;
                place = i;
                replacmets = 1;
            }
        }
    }
    if(replacmets > 0){
        ram[place].page_number = page;
        ram[place].page_time_in = time;
        ram[place].last_time_referanced = time;
        page_t[tp].v_bit = 0;
        page_t[page].v_bit = 1;
    }else{
        //printf("Error!\nSomething went wrong!\n");
    }
}
