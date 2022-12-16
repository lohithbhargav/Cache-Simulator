/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "cachesim.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <direct, n-way, fully or L1L2> <trace file name>\n", argv[0]);
        return 1;
    }

#ifdef DBG
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
    printf("\n");
#endif

    char* trace_file_name = argv[2];
    struct n_way_associative_cache n_cache;
    struct n_way_associative_cache n_cache_L1;
    struct n_way_associative_cache n_cache_L2;
    struct fully_associative_cache f_cache;
    struct direct_mapped_cache d_cache;
    char mem_request[20];
    uint64_t address;
    FILE *fp;
    srand((unsigned int)time(NULL));



    /* Opening the memory trace file */
    fp = fopen(trace_file_name, "r");

    if (strncmp(argv[1], "direct", 6)==0) { /* Simulating direct-mapped cache */
        
        /* Initialization */
        for (int i=0; i<NUM_BLOCKS; i++) {
            d_cache.valid_field[i] = 0;
            d_cache.dirty_field[i] = 0;
            d_cache.tag_field[i] = 0;
        }
        d_cache.hits = 0;
        d_cache.misses = 0;
        
        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            direct_mapped_cache_access(&d_cache, address);
        }
        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    Direct-Mapped Cache\n");
        printf("==================================\n");
        printf("Cache Hits:    %d\n", d_cache.hits);
        printf("Cache Misses:  %d\n", d_cache.misses);
        float total = d_cache.hits + d_cache.misses;
        printf("==================================\n");
        printf("Cache Hit Rate: %0.2f\n", (d_cache.hits / total));
        printf("Cache Miss Rate: %0.2f\n", (d_cache.misses / total));
        printf("\n");
    }
    else if (strncmp(argv[1], "n-way", 5)==0) { /* Simulating n-way associative cache */
        
        /* Initialization */
        for (int i=0; i<NUM_SETS; i++) {
            for (int j=0; j<WAY_SIZE; j++){
                n_cache.valid_field[i][j] = 0;
                n_cache.dirty_field[i][j] = 0;
                n_cache.tag_field[i][j] = 0;
                n_cache.set_address[i][j] = 0;
            }
        }
        n_cache.hits = 0;
        n_cache.misses = 0;
        
        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            n_way_associative_cache_access(&n_cache, address);
        }
        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    N-Way Associative Cache\n");
        printf("==================================\n");
        printf("Cache Hits:    %d\n", n_cache.hits);
        printf("Cache Misses:  %d\n", n_cache.misses);
        float total = n_cache.hits + n_cache.misses;
        printf("==================================\n");
        printf("Cache Hit Rate: %0.2f\n", (n_cache.hits / total));
        printf("Cache Miss Rate: %0.2f\n", (n_cache.misses / total));
        printf("\n");
    }
    else if (strncmp(argv[1], "fully", 5)==0) { /* Simulating fully associative cache */
        
        /* Initialization */
        for (int i=0; i<NUM_BLOCKS; i++) {
            f_cache.valid_field[i] = 0;
            f_cache.dirty_field[i] = 0;
            f_cache.tag_field[i] = 0;
            f_cache.block_address[i] = 0;
        }
        f_cache.hits = 0;
        f_cache.misses = 0;
        
        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            fully_associative_cache_access(&f_cache, address);
        }
        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    Fully Associative Cache\n");
        printf("==================================\n");
        printf("Cache Hits:    %d\n", f_cache.hits);
        printf("Cache Misses:  %d\n", f_cache.misses);
        float total = f_cache.hits + f_cache.misses;
        printf("==================================\n");
        printf("Cache Hit Rate: %0.2f\n", (f_cache.hits / total));
        printf("Cache Miss Rate: %0.2f\n", (f_cache.misses / total));
        printf("\n");
    }
    else if (strncmp(argv[1], "L1L2", 4)==0){ /* Simulating n-way-associative-mapped cache */
        /* Initialization */
        for (int i=0; i<NUM_SETS_L1; i++) {
            for (int j=0; j<WAY_SIZE_L1; j++){
                n_cache_L1.valid_field[i][j] = 0;
                n_cache_L1.dirty_field[i][j] = 0;
                n_cache_L1.tag_field[i][j] = 0;
                n_cache_L1.set_address[i][j] = 0;
            }
        }
        n_cache_L1.hits = 0;
        n_cache_L2.misses = 0;

        /* Initialization */
        for (int i=0; i<NUM_SETS_L2; i++) {
            for (int j=0; j<WAY_SIZE_L2; j++){
                n_cache_L2.valid_field[i][j] = 0;
                n_cache_L2.dirty_field[i][j] = 0;
                n_cache_L2.tag_field[i][j] = 0;
                n_cache_L2.set_address[i][j] = 0;
            }
        }
        n_cache_L2.hits = 0;
        n_cache_L2.misses = 0;

        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp) != NULL)
        {
            address = convert_address(mem_request);
            L1_L2_cache_access(&n_cache_L1, &n_cache_L2, address);
        }

        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    L1 L2 Cache\n");
        printf("==================================\n");
        printf("Cache L1 Hits:    %d\n", n_cache_L1.hits);
        printf("Cache L1 Misses:  %d\n", n_cache_L1.misses);
        float L1_total = n_cache_L1.hits + n_cache_L1.misses;
        printf("==================================\n");
        printf("Cache L1 Hit Rate: %0.2f\n", (n_cache_L1.hits / L1_total));
        printf("Cache L1 Miss Rate: %0.2f\n", (n_cache_L1.misses / L1_total));
        printf("\n");
        int total = n_cache_L1.hits + n_cache_L1.misses + n_cache_L2.hits + n_cache_L2.misses;
        printf("==================================\n");
        printf("Total CPU requested memory access: %d\n", total);
        printf("\n");
        printf("Cache L2 Hits:    %d\n", n_cache_L2.hits);
        printf("Cache L2 Misses:  %d\n", n_cache_L2.misses);
        printf("==================================\n");
        float L2_total = n_cache_L2.hits + n_cache_L2.misses;
        printf("Cache L2 Local Hit Rate: %0.2f\n", (n_cache_L2.hits / L2_total));
        printf("Cache L2 Local Miss Rate: %0.2f\n", (n_cache_L2.misses / L2_total));
        float ttotal = total;
        printf("Cache L2 Global Hit Rate: %0.2f\n", 1 - (n_cache_L2.misses / L1_total));
        printf("Cache L2 Global Miss Rate: %0.2f\n", (n_cache_L2.misses / L1_total));
        printf("\n");

    }

    fclose(fp);

    return 0;
}

uint64_t convert_address(char memory_addr[])
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it) */
{
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n') {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0') {
            binary = (binary*16) + (memory_addr[i] - '0');
        } else {
            if(memory_addr[i] == 'a' || memory_addr[i] == 'A') {
                binary = (binary*16) + 10;
            }
            if(memory_addr[i] == 'b' || memory_addr[i] == 'B') {
                binary = (binary*16) + 11;
            }
            if(memory_addr[i] == 'c' || memory_addr[i] == 'C') {
                binary = (binary*16) + 12;
            }
            if(memory_addr[i] == 'd' || memory_addr[i] == 'D') {
                binary = (binary*16) + 13;
            }
            if(memory_addr[i] == 'e' || memory_addr[i] == 'E') {
                binary = (binary*16) + 14;
            }
            if(memory_addr[i] == 'f' || memory_addr[i] == 'F') {
                binary = (binary*16) + 15;
            }
        }
        i++;
    }

#ifdef DBG
    printf("%s converted to %llu\n", memory_addr, binary);
#endif
    return binary;
}

void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif

    if (cache->valid_field[index] && cache->tag_field[index] == tag) { /* Cache hit */
        cache->hits += 1;
#ifdef DBG
        printf("Hit!\n");
#endif
    } else {
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[index] && cache->dirty_field[index]) {
            /* Write the cache block back to memory */
        }
        cache->tag_field[index] = tag;
        cache->valid_field[index] = 1;
        cache->dirty_field[index] = 0;
    }
}

void n_way_associative_cache_access(struct n_way_associative_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);
    uint64_t set = index % NUM_SETS;
    int flag = -1; /* Cache Miss -1 or Cache Hit flag = index*/
    int random = rand() % WAY_SIZE; /* for random replacement policy */
    int empty = -1; /* cache index for an empty block */

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif
    for (int i=0; i<WAY_SIZE; i++) {
        if (cache->tag_field[set][i] == tag && cache->set_address[set][i] == index && cache->valid_field[set][i]) { /* Cache hit */
            cache->hits += 1; /* cache hit */
            flag = i;
#ifdef DBG
        printf("Hit!\n");
#endif
        break;
    } 
    if (!cache->valid_field[set][i]) {
        empty = i;
    }
    }
    if (flag == -1) {
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[set][flag] && cache->dirty_field[set][flag]) {
            /* Write the cache block back to memory */
        }
        if (empty == -1) { /* Cache capacity was full then replace random block */
            cache->tag_field[set][random] = tag;
            cache->valid_field[set][random] = 1;
            cache->dirty_field[set][random] = 0;
            cache->set_address[set][random] = index;
        }
        else { /* Cache not full then write into empty block */
            cache->tag_field[set][empty] = tag;
            cache->valid_field[set][empty] = 1;
            cache->dirty_field[set][empty] = 0;
            cache->set_address[set][empty] = index;
        }
    }
}

void fully_associative_cache_access(struct fully_associative_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);
    int flag = -1; /* Cache Miss -1 or Cache Hit flag = index*/
    int random = rand() % NUM_BLOCKS; /* for random replacement policy */
    int empty = -1; /* int minimum value */

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif

    for (int i=0;i<NUM_BLOCKS;i++) {
        if (cache->tag_field[i] == tag && cache->block_address[i] == index && cache->valid_field[i]) { /* Cache hit */
            cache->hits += 1;
            flag = i;
#ifdef DBG
        printf("Hit!\n");
#endif
        break;
    } else if (!cache->valid_field[i]) {
            empty = i;
        }
    }
    if (flag == -1){
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[flag] && cache->dirty_field[flag]) {
            /* Write the cache block back to memory */
        }
        if (empty == -1) { /* Cache capacity was full then replace random block */
            cache->tag_field[random] = tag;
            cache->valid_field[random] = 1;
            cache->dirty_field[random] = 0;
            cache->block_address[random] = index;
        }
        else { /* Cache not full then write into empty block */
            cache->tag_field[empty] = tag;
            cache->valid_field[empty] = 1;
            cache->dirty_field[empty] = 0;
            cache->block_address[empty] = index;
        }
    }
}

void L1_L2_cache_access(struct n_way_associative_cache *cache1, struct n_way_associative_cache *cache2, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index1 = block_addr % NUM_BLOCKS_L1;
    uint64_t tag1 = block_addr >> (unsigned)log2(NUM_BLOCKS_L1);
    uint64_t set1 = index1 % NUM_SETS_L1;
    uint64_t index2 = block_addr % NUM_BLOCKS_L2;
    uint64_t tag2 = block_addr >> (unsigned)log2(NUM_BLOCKS_L2);
    uint64_t set2 = index2 % NUM_SETS_L2;
    int flag1 = -1; /* Cache Miss -1 or Cache Hit flag = index*/
    int random1 = rand() % WAY_SIZE_L1; /* for random replacement policy */
    int empty1 = -1; /* cache index for an empty block */
    int flag2 = -1; /* Cache Miss -1 or Cache Hit flag = index*/
    int random2 = rand() % WAY_SIZE_L2; /* for random replacement policy */
    int empty2 = -1; /* cache index for an empty block */

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif
    for (int i=0; i<WAY_SIZE_L1; i++) {
        if (cache1->tag_field[set1][i] == tag1 && cache1->set_address[set1][i] == index1 && cache1->valid_field[set1][i]) { /* Cache hit */
            cache1->hits += 1; /* cache hit */
            flag1 = i;
#ifdef DBG
        printf("Hit!\n");
#endif
        break;
    } 
    if (!cache1->valid_field[set1][i]) {
        empty1 = i;
    }
    }
    if (flag1 == -1) {
        /* Cache miss */
        cache1->misses += 1;
        for (int i=0; i<WAY_SIZE_L2; i++) {
        if (cache2->tag_field[set2][i] == tag2 && cache2->set_address[set2][i] == index2 && cache2->valid_field[set2][i]) { /* Cache hit */
            cache2->hits += 1; /* cache hit */
            flag2 = i;
#ifdef DBG
        printf("Hit!\n");
#endif
        if (empty1 == -1){ /* for random replacement */
            cache1->tag_field[set1][random1] = tag1;
            cache1->valid_field[set1][random1] = 1;
            cache1->dirty_field[set1][random1] = 0;
            cache1->set_address[set1][random1] = index1;
        }
        else{
            /* cache not full, add to empty block */
            cache1->tag_field[set1][empty1] = tag1;
            cache1->valid_field[set1][empty1] = 1;
            cache1->dirty_field[set1][empty1] = 0;
            cache1->set_address[set1][empty1] = index1;
            }
        break;
    } 
    if (!cache2->valid_field[set2][i]) {
        empty2 = i;
    }
    }
    if (flag2 == -1){
        /* Cache miss */
        cache2->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache2->valid_field[set2][flag2] && cache2->dirty_field[set2][flag2]) {
            /* Write the cache block back to memory */
        }
        if (empty2 == -1) { /* Cache capacity was full then replace random block */
            cache2->tag_field[set2][random2] = tag2;
            cache2->valid_field[set2][random2] = 1;
            cache2->dirty_field[set2][random2] = 0;
            cache2->set_address[set2][random2] = index2;
        }
        else { /* Cache not full then write into empty block */
            cache2->tag_field[set2][empty2] = tag2;
            cache2->valid_field[set2][empty2] = 1;
            cache2->dirty_field[set2][empty2] = 0;
            cache2->set_address[set2][empty2] = index2;
        }
    }
}
}
