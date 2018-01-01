#include "ordered_array.h"
#include "kheap.h"
#include "system.h"

ordered_array_t ordered_array_create(uint32_t max_size, compare_predicate_t comparison)
{
	void* heap_address = (void*)kmalloc(max_size * sizeof(type_t));
	return ordered_array_place(max_size, comparison, heap_address);
}

ordered_array_t ordered_array_place(uint32_t max_size, compare_predicate_t comparison, void *address)
{
	ordered_array_t new;
	new.array = (type_t*)address;
	memset(new.array, 0, max_size * sizeof(type_t));
	new.size = 0;
	new.max_size = max_size;
	new.comparison = comparison;
	return new;
}

void ordered_array_destroy(ordered_array_t *array)
{
	UNUSED_VAR(array);
	// TODO: free array->array
	PANIC("not implemented: ordered_array_destroy");
}

void ordered_array_insert(type_t item, ordered_array_t *array)
{
	// Check if the array is already at maximum capacity
	if (array->size == array->max_size)
	{
		// TODO: return a failure code instead of panic?
		PANIC("ordered array at max size");
	}

	// Walk along array until we reach the correct insertion point
	uint32_t index = 0;
	while (index < array->size && array->comparison(array->array[index], item) < 0)
	{
		index++;
	}

	if (index == array->size) // reached end of array
	{
		// Add to end of array
		array->array[array->size++] = item;
	}
	else
	{
		// Save existing item
		type_t prev = array->array[index];
		// Insert item at this index
		array->array[index] = item;
		// Move all existing elements along
		while (index < array->size)
		{
			index++;
			type_t current = array->array[index];
			array->array[index] = prev;
			prev = current;
		}
		array->size++;
	}
}

type_t ordered_array_get(uint32_t index, ordered_array_t *array)
{
	return array->array[index];
}

void ordered_array_remove(uint32_t index, ordered_array_t *array)
{
	for (; index < array->size; index++)
	{
		array->array[index] = array->array[index + 1];
	}
	array->size--;
}