clang -shared -undefined dynamic_lookup -o libhashmap.so -U__ANDROID_API__ -D__ANDROID_API__=34 -D__ANDROID_UNAVAILABLE_SYMBOLS_ARE_WEAK__ hash_map.c; 
clang -L . -o test_hash_map test.c ./libhashmap.so
