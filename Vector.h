/** \file Vector.h
 *
 * \mainpage LK Vector
 * 
 * Describes the API used to interact with Vector objects.
 *
 * \section Description
 *
 * A Vector is a dynamically resizable array of data. The type stored is 
 * irrelevant in this implementation, as it uses the size of the data 
 * only. 
 *
 * Usually, though, it is recommended to store only one type in a given
 * Vector instance.
 *
 * A Vector can be resized and supports \ref vec_push_back "push_back", which will take care
 * of resizing to fit new data. Random access is supported, too!
 */

#ifndef LK_VECTOR_H
#define LK_VECTOR_H

#include <stdlib.h>

#define VERSION_STRING "LK Vector library v0.1"

typedef enum VectorDataType
{
    VEC_FLOAT,
    VEC_INT,
} VectorDataType;

/// \struct Vector
/// \brief A dynamically resizable array
typedef struct Vector {
    /// \brief pointer to the raw data.
    void* data;
    /// \brief size (in bytes) of the type the vector is holding
    size_t type_size;
    /// \brief amount of elements currently stored
    size_t size;
    /// \brief amount of elements this vector can fit before resizing itself
    size_t capacity;

} Vector;

/// \brief Creates an empty vector with zeroed-out properties.
/// Note the resulting object is pretty much invalid to any of the vec_* functions.
Vector* vec_create_empty();
/// \brief Creates a vector with space to fit "capacity" many objects of "type".
/// Creates a vector with enough space reserved to fit the amount (capacity)
/// of objects of the given type (use vec_create_size if this doesn't provide
/// the needed size).
Vector* vec_create_type(size_t capacity, VectorDataType type);
/// \brief Creates a vector with capacity*type_size bytes reserved.
/// Creates a vector with enough reserved space to fit as many objects of size
/// type_size as specified by capacity.
Vector* vec_create_size(size_t capacity, size_t type_size);

/// \brief Returns a pointer to the object at the given index in the vector.
void* vec_at(Vector* v, size_t i);
/// \brief Returns the float at the given index in the vector.
float vec_at_f(Vector* v, size_t i);
/// \brief Returns the int at the given index in the vector.
int vec_at_i(Vector* v, size_t i);

/// \brief Appends data to the vector.
/// It is assumed that data has the exact size "type_size" specified during the
/// creation of the vector. Otherwise the behaviour is undefined.
/// This operation will resize the vector if needed.
void vec_push_back(Vector* v, void* data);
/// \brief Appends an int to the vector.
/// Will resize the vector if needed.
void vec_push_back_i(Vector* v, int data);
/// \brief Appends a float to the vector.
/// Will resize the vector if needed.
void vec_push_back_f(Vector* v, float data);

/// \brief Resizes the vector to fit a given count.
/// Ensures that the vector has enough space to fit "size" many objects
/// of size v->type_size or more. The vector will remain unchanged if it
/// can already hold the given amount of objects.
void vec_reserve(Vector* v, size_t size);
/// \brief Grows the vector's reserved space by "amount".
void vec_reserve_grow(Vector* v, size_t amount);

/// \brief Pretty-prints the vector's properties and data.
void vec_debugprint(Vector* v);

/// \brief Frees the vector's data and the vector itself, then sets the pointer to NULL.
#define vec_free(v)        \
    do {                   \
        if (v)             \
            free(v->data); \
        free(v);           \
        v = NULL;          \
    } while (0)


#endif // LK_VECTOR_H
