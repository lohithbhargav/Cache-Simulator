/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#define BLOCK_SIZE  64        /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
#define WAY_SIZE    1         /* Associativity; 1-way = direct-mapped */
#define CACHE_SIZE  32768     /* Cache capacity in bytes (must be power of 2)*/

#define NUM_BLOCKS  (CACHE_SIZE / BLOCK_SIZE)
#define NUM_SETS    (NUM_BLOCKS / WAY_SIZE)

/*L1 Cache inputs*/
#define BLOCK_SIZE_L1 64
#define CACHE_SIZE_L1 65536
#define NUM_BLOCKS_L1 (CACHE_SIZE_L1 / BLOCK_SIZE_L1)
#define WAY_SIZE_L1 2
#define NUM_SETS_L1 (NUM_BLOCKS_L1/ WAY_SIZE_L1)

/*L2 Cache inputs*/
#define BLOCK_SIZE_L2 64
#define CACHE_SIZE_L2 1048576
#define NUM_BLOCKS_L2 (CACHE_SIZE_L2 / BLOCK_SIZE_L2)
#define WAY_SIZE_L2 8
#define NUM_SETS_L2 (NUM_BLOCKS_L2/ WAY_SIZE_L2)

//#define DBG

/*The data structure of direct-mapped cache*/
struct direct_mapped_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};

/*The data structure of n-way associative cache*/
struct n_way_associative_cache {
    unsigned valid_field[NUM_SETS][WAY_SIZE];   /* Valid field */
    unsigned dirty_field[NUM_SETS][WAY_SIZE];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_SETS][WAY_SIZE];     /* Tag field */
    uint64_t set_address[NUM_SETS][WAY_SIZE];  /* Set address */
    char data_field[NUM_SETS][WAY_SIZE][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};

/*The data structure of fully associative cache*/
struct fully_associative_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    uint64_t block_address[NUM_BLOCKS];  /* Block address */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};

/*Read the memory traces and convert it to binary*/
uint64_t convert_address(char memory[]);

/*Simulate the direct-mapped cache*/
void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address);
/*Simulate the n-way associative cache*/
void n_way_associative_cache_access(struct n_way_associative_cache *cache, uint64_t address);
/*Simulate the fully associative cache*/
void fully_associative_cache_access(struct fully_associative_cache *cache, uint64_t address);
/*Simulate the L1L2 cache*/
void L1_L2_cache_access(struct n_way_associative_cache *cache1, struct n_way_associative_cache *cache2, uint64_t address);
