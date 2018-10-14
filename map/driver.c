#include <stdio.h>

#include "map.h"

void hashtable_print(map *m);

int main(void)
{
	map *abram = map_create();

	if(map_insert(abram, "Golf", 180)) {
		puts("Inserted 180 at Golf");
	}

	if(map_exists(abram, "Golf")) {
		puts("Golf Exists");
	}

	if(map_exists(abram, "India")) {
		puts("India EXISTS");
	} else {
		puts("India does not exist");
	}

	printf("Golf: %lf\n", map_lookup(abram, "Golf"));

	// Exercises collision resolution
	map_insert(abram, "27", 28);
	map_insert(abram, "1X", 10);
	hashtable_print(abram);

	map_insert(abram, "benson", 38);
	map_insert(abram, "cain", 39);
	map_insert(abram, "feese", 44);
	map_insert(abram, "fossett", 36);
	map_insert(abram, "hughes", 42);
	map_insert(abram, "jackson", 41);
	map_insert(abram, "latane", 35);
	map_insert(abram, "lohmeyer", 31);
	map_insert(abram, "longwell", 37);
	map_insert(abram, "miramontes", 30);
	map_insert(abram, "shea", 32);
	map_insert(abram, "spence", 33);
	map_insert(abram, "wellman", 34);

	hashtable_print(abram);

	map_destroy(abram);
}
