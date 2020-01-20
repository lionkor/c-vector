#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Vector.h"

void vec_reserve(Vector* v, size_t size) {
    // we can return if we already have that much or more space reserved
    if (v->capacity >= size)
        return;
}

void vec_debugprint(Vector* v) {
    puts(__FUNCTION__);
    if (v == NULL) {
        printf("Vector* is NULL\n");
        return;
    }
    printf("Vector at %p\n", v);
    printf(" size      = %5lu\n", v->size);
    printf(" capacity  = %5lu\n", v->capacity);
    printf(" type_size = %5lu\n", v->type_size);
    printf(" data (%p) =\n {\n", v->data);
    for (size_t i = 0; i < v->size; ++i)
        printf("   [%lu]   %11i\n", i, vec_at_i(v, i));
    printf(" }\n");
}

void vec_push_back(Vector* v, void* data) {
    if (v->size >= v->capacity) {
        ++v->capacity;
        v->data = realloc(v->data, v->capacity * v->type_size);
    }
    // .75
    // .015
    memcpy((char*)v->data + v->size * v->type_size, data, v->type_size);
    ++v->size;
}

void vec_push_back_i(Vector* v, int data) {
    vec_push_back(v, (void*)&data);
}
void vec_push_back_f(Vector* v, float data) {
    vec_push_back(v, (void*)&data);
}

void* vec_at(Vector* v, size_t i) {
    return v->data + i * v->type_size;
}

float vec_at_f(Vector* v, size_t i) {
    return *(float*)vec_at(v, i);
}

int vec_at_i(Vector* v, size_t i) {
    return *(int*)vec_at(v, i);
}

Vector* vec_create_empty() {
    Vector* v    = (Vector*)malloc(sizeof(Vector));
    v->data      = NULL;
    v->type_size = 0;
    v->size      = 0;
    v->capacity  = 0;
    return v;
}

Vector* vec_create_size(size_t capacity, size_t type_size) {
    // allowing type_size of 0 will definitely break everything, so let's not
    // allow that.
    if (type_size == 0)
        return NULL;

    Vector* v = (Vector*)malloc(sizeof(Vector));

    // its ok to pass 0 to calloc, but we can't realloc the result of
    // that, so we need to check for this and return NULL
    if (capacity == 0)
        v->data = NULL;
    else
        v->data = calloc(capacity, type_size);
    v->capacity  = capacity;
    v->size      = 0;
    v->type_size = type_size;
    return v;
}

Vector* vec_create_type(size_t capacity, VectorDataType type) {
    switch (type) {
    case VEC_INT:
        return vec_create_size(capacity, sizeof(int));
        break;
    case VEC_FLOAT:
        return vec_create_size(capacity, sizeof(float));
        break;
    }
    // NOT IMPLEMENTED
    return NULL;
}

