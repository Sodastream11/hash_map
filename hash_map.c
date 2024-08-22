#include "hash_map.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/random.h>



hash_map* hm_alloc(const size_t n) {
    hash_map* map = calloc(1, sizeof(hash_map));
    map->size = n;

    map->ptr = calloc(map->size, sizeof(map_entry*));

    uint32_t seed;
    getrandom(&seed, sizeof(uint32_t), 0);
    map->seed = seed;

    return map;
}


void hm_set(hash_map* map, const void* key, const float val) {
    map_entry new_entry = { .key = key, .val = val, .next = NULL };
    uint32_t idx = hash(key, strlen(key), map->seed) % map->size;

    map_entry *entry;
    if(entry = map->ptr[idx], entry != NULL) {
        new_entry.next = entry;
    } 

    map->ptr[idx] = &new_entry;
}

float hm_get(const hash_map* map, const void* key) {
    uint32_t idx = hash(key, strlen(key), map->seed) % map->size;

    map_entry* entry = map->ptr[idx];
    while (entry = entry->next, entry != NULL) {
        if (!strcmp(entry->key, key)) {
            break;
        }
    }

    return entry->val;
}

void hm_free(const hash_map* map) {
    for (int i = 0; i < map->size; i++) {
        free_chain(*(map->ptr + i));
        map->ptr[i] = NULL;
    }
    free(map);
}

void free_chain(map_entry* entry) {
    if (entry->next != NULL) {
        free_chain(entry->next);
        entry->next = NULL;
    }
    free(entry);
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
