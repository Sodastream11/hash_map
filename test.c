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

    double get_val = hm_get(map, "testkey");
    if (get_val != 1.0) {
        printf("wrong get value for key %s: %f, should be: %f \n", "testkey", get_val, 1.0);
    }

    printf("got value for key '%s': %f\n", "testkey", get_val);
    get_val = hm_get(map, "fuck, this key doesn't exist");
    printf("got value for key '%s': %f\n", "fuck, this key doesn't exist", get_val);

    hm_free(map);
    return 0;
}
