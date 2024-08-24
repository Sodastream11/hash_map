#include <stdio.h>
#include "hash_map.h"

int main() {
    setbuf(stdout, NULL);
    hash_map* map = hm_alloc(10);
    hm_set(map, "testkey", 1.0);
    hm_set(map, "yo", 2.0);
    hm_set(map, "hello", 3.0);
    hm_set(map, "whats", 4.0);
    hm_set(map, "up", 5.0);
    hm_set(map, "bro", 6.0);
    puts("map before resize:\n");
    hm_print(map);

    hm_set(map, "is_it_gonna_work?", 7.0);
    puts("map after resize:\n");
    hm_print(map);

    hm_free(map);
    return 0;
}
