#include <stddef.h>
#include <stdint.h>


typedef struct map_entry {
    char * key;
    uint32_t val;
    map_entry * next; // for collisions
} map_entry;

typedef struct {
    map_entry ** ptr;
    size_t size;
    uint32_t seed;
} hash_map;


hash_map* hm_alloc(const size_t n);

void hm_set(hash_map* map, const void* key, const float val);

float hm_get(const hash_map* map, const void* key);

//TODO
map_entry* hm_collect_entries(const hash_map* map);

void hm_resize(hash_map* map, const size_t s);

uint32_t hash(const void* key, int n, uint32_t seed);

void hm_free(const hash_map* map);
void free_chain(map_entry* entry);
