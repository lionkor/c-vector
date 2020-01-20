#ifndef LK_VECTOR_H
#define LK_VECTOR_H

#include <stdlib.h>

#define VERSION_STRING "LK Vector library v0.1"

typedef enum VectorDataType
{
    VEC_FLOAT,
    VEC_INT,
} VectorDataType;

typedef struct Vector {
    void*  data;
    size_t type_size;
    size_t size;
    size_t capacity;
} Vector;

// Creates an empty vector with zeroed-out properties. Note that data is NULL, so
// you cannot append to this or do any other operations on it.
Vector* vec_create_empty();
// Creates a vector with enough space reserved to fit the amount (capacity)
// of objects of the given type (use vec_create_size if this doesn't provide
// the needed size).
Vector* vec_create_type(size_t capacity, VectorDataType type);
// Creates a vector with enough reserved space to fit as many objects of size
// type_size as sepcified by capacity.
Vector* vec_create_size(size_t capacity, size_t type_size);

// Returns a pointer to the object at the given index in the vector.
void* vec_at(Vector* v, size_t i);
// Returns the float at the given index in the vector.
float vec_at_f(Vector* v, size_t i);
// Returns the int at the given index in the vector.
int vec_at_i(Vector* v, size_t i);

// Appends data to the vector. It is assumed that data has the
// exact size "type_size" specified during the creation of the vector.
void vec_push_back(Vector* v, void* data);
// Appends an int to the vector.
void vec_push_back_i(Vector* v, int data);
// Appends a float to the vector.
void vec_push_back_f(Vector* v, float data);

// Ensures that the vector has enough space to fit "size" many objects
// of size v->type_size or more.
void vec_reserve(Vector* v, size_t size);
// Grows the vector's reserved space by "amount".
void vec_reserve_grow(Vector* v, size_t amount);

// Pretty-prints the vector's properties and data.
void vec_debugprint(Vector* v);

// Frees the vector's data and the vector itself, then sets the pointer to NULL.
#define vec_free(v)        \
    do {                   \
        if (v)             \
            free(v->data); \
        free(v);           \
        v = NULL;          \
    } while (0)


#endif // LK_VECTOR_H
