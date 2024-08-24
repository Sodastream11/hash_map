#include "hash_map.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <sys/random.h>


hash_map* hm_alloc(const size_t size) {
    hash_map* map = calloc(1, sizeof(hash_map));
    map->n = 0;

    map->chain_arr = calloc(size, sizeof(map_entry*));
    map->size = size;

    uint32_t seed;
    getrandom(&seed, sizeof(uint32_t), 0);
    map->seed = seed;

    return map;
}

void hm_resize(hash_map *map, const size_t size) {
#ifdef DEBUG
    printf("resizing map to size: %d\n", size);
#endif
    map_entry** new_chain_arr = calloc(size, sizeof(map_entry*));

    for (int i = 0; i < map->size; i++) {
#ifdef DEBUG
        printf("moving nodes of chain: %d to new array\n", i);
#endif
        if (map->chain_arr[i] != NULL) {
            chain_move_to_new_arr(map->chain_arr[i], new_chain_arr, size);
            map->chain_arr[i] = NULL;
        }
    }

    free(map->chain_arr);
    map->size = size;
    map->chain_arr = new_chain_arr;
}

void chain_move_to_new_arr(map_entry* chain, map_entry** new_chain_arr, const size_t new_size) {
    if (chain->next != NULL) {
        chain_move_to_new_arr(chain->next, new_chain_arr, new_size);
    }

    map_entry* to_copy = chain;
    to_copy->next = NULL;

    uint32_t new_idx = to_copy->hash % new_size;

    map_entry *existing;
    if(existing = new_chain_arr[new_idx], existing != NULL) {
        to_copy->next = existing;
    } 

    new_chain_arr[new_idx] = to_copy;
}


void hm_set(hash_map* map, const void* key, const double val) {
#ifdef DEBUG
    printf("setting key: %s with value: %f\n", key, val);
#endif
    map_entry* new_entry = calloc(1, sizeof(map_entry));
    *new_entry = (map_entry) { .key = key, .val = val, .next = NULL , .hash = hash(key, strlen(key), map->seed)};
    uint32_t idx = new_entry->hash % map->size;
#ifdef DEBUG
    printf("idx: %lu with hash: %lu\n", idx, new_entry->hash);
#endif
    map_entry *entry;
    if(entry = map->chain_arr[idx], entry != NULL) {
        new_entry->next = entry;
    } 

    map->chain_arr[idx] = new_entry;
    map->n++;
    
#ifdef DEBUG
    printf("checking load factor with n: %lu size: %lu\n", map->n,  map->size);
#endif
    if ((double)map->n / (double)map->size >= LOAD_FACTOR_THRESHOLD) {
        hm_resize(map, ceil((double)map->n/LOAD_FACTOR_RESET));
    }
}

double hm_get(const hash_map* map, const void* key) {
    uint32_t idx =  hash(key, strlen(key), map->seed) % map->size;
    

    map_entry* entry;
    LINKED_LIST_MAP(map->chain_arr[idx], entry, 
        if (!strcmp(entry->key, key)) {
            break;
        }
    )
    if (entry == NULL) {
        return 0; // TODO: make errors more clear
    }
    return entry->val;
}

void hm_print(const hash_map* map) {
    printf("size: %d\n", map->size);
    printf("amount of pairs: %d\n", map->n);
    printf("seed: %lu\n", map->seed);
    for (int i = 0; i < map->size; i++) {
        map_entry* entry;
        LINKED_LIST_MAP(map->chain_arr[i], entry, printf(" -- %f", entry->val);)
        printf("\n|\n");
    }
}

void hm_free(hash_map* map) {
    for (int i = 0; i < map->size; i++) {
#ifdef DEBUG
        printf("freeing chain with idx: %d\n", i);
#endif
        if (map->chain_arr[i] != NULL) {
            chain_free(map->chain_arr[i]);
            map->chain_arr[i] = NULL;
        }
    }
    free(map->chain_arr);
    map->chain_arr = NULL;

    free(map);
}

void chain_free(map_entry* chain) {
#ifdef DEBUG
    printf("freeing chain recursively: %x\n", chain);
#endif
    if (chain->next != NULL) {
#ifdef DEBUG
        printf("    recursive call\n");
#endif
        chain_free(chain->next);
        chain->next = NULL;
    }
    free(chain);
#ifdef DEBUG
    printf("freed chain recursively\n");
#endif
}


// Murmur hash 2:
// https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c
uint32_t hash(const void *key, int n, uint32_t seed) {

    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    uint32_t h = seed ^ n;

    const unsigned char *data = (const unsigned char *)key;

    while (n >= 4) {
        uint32_t k = *(uint32_t *)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        n -= 4;
    }

    switch (n) {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}
