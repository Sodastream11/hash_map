#include <stddef.h>
#include <stdint.h>

typedef struct map_entry {
    char * key;
    double val;
    struct map_entry * next; // for collisions
    uint32_t hash; // un-modded hash value, used for resizing
} map_entry;

typedef struct {
    map_entry ** chain_arr;
    uint32_t n;
    size_t size;
    uint32_t seed;
} hash_map;


#define LOAD_FACTOR_THRESHOLD 0.7
#define LOAD_FACTOR_RESET 0.5

#define LINKED_LIST_ITER(list, iter_ptr, expr) \
    iter_ptr = list;               \
    while(iter_ptr != NULL) {      \
        expr                       \
        iter_ptr = iter_ptr->next; \
    }                              \
                                                

uint32_t hash(const void* key, int n, uint32_t seed);

hash_map* hm_alloc(const size_t size);

void hm_set(hash_map* map, const void* key, const double val);

double hm_get(const hash_map* map, const void* key);
//TODO
map_entry* hm_collect_entries(const hash_map* map);

void hm_print(const hash_map* map);

void hm_resize(hash_map* map, const size_t s);
void chain_move_to_new_arr(map_entry* chain, map_entry** new_chain_arr, const size_t new_size);


void hm_free(hash_map* map);
void chain_free(map_entry* chain);
