libhashmap.so: hash_map.c hash_map.h
	clang -I -shared -undefined dynamic_lookup -o libhashmap.so hash_map.c 
