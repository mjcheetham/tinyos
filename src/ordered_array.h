#pragma once
#include "types.h"

// General type of elements in an ordered array.
typedef void* type_t;

// Comparison predicate for ordering the elements of the array.
// Returns negative if arg0 < arg1, positive if arg0 > arg1, and zero if equal.
typedef int8_t (*compare_predicate_t)(type_t, type_t);

typedef struct
{
	type_t *array;
	uint32_t size;
	uint32_t max_size;
	compare_predicate_t comparison;
} ordered_array_t;

// Create an ordered array on the heap.
ordered_array_t ordered_array_create(uint32_t max_size, compare_predicate_t comparison);

// Create an ordered array at the given address.
ordered_array_t ordered_array_place(uint32_t max_size, compare_predicate_t comparison, void *address);

// Destroy an ordered array.
void ordered_array_destroy(ordered_array_t *array);

// Insert an item into an ordered array.
void ordered_array_insert(type_t item, ordered_array_t *array);

// Get the item at the given index in an ordered array.
type_t ordered_array_get(uint32_t index, ordered_array_t *array);

// Remove the item at the given index in an ordered array.
void ordered_array_remove(uint32_t index, ordered_array_t *array);
