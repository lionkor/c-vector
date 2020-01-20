#include "acutest/include/acutest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

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

Vector* vec_create_empty();
Vector* vec_create_type(size_t capacity, VectorDataType type);
Vector* vec_create_size(size_t capacity, size_t type_size);
void    vec_free(Vector* v);

void* vec_at(Vector* v, size_t i);
float vec_at_f(Vector* v, size_t i);
int   vec_at_i(Vector* v, size_t i);

void vec_push_back(Vector* v, void* data);
void vec_push_back_i(Vector* v, int data);
void vec_push_back_f(Vector* v, float data);

void vec_push_back(Vector* v, void* data) {
    if (v->size >= v->capacity) {
        ++v->capacity;
        v->data = realloc(v->data, v->capacity * v->type_size);
    }
    memcpy(&((char*)v->data)[v->size * v->type_size], data, v->type_size);
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
    assert(false);
}

#define vec_free(v)           \
    {                         \
        if (v && v->capacity) \
            free(v->data);    \
        free(v);              \
        v = NULL;             \
    }

void veci(void) {
    TEST_CASE("enough capacity");
    Vector* int_vec = vec_create_type(5, VEC_INT);
    TEST_CHECK(int_vec != NULL);
    vec_push_back_i(int_vec, 5);
    vec_push_back_i(int_vec, -5);
    TEST_CHECK(vec_at_i(int_vec, 0) == 5);
    TEST_CHECK(vec_at_i(int_vec, 1) == -5);
    vec_free(int_vec);

    TEST_CASE("no capacity");
    int_vec = vec_create_type(0, VEC_INT);
    TEST_CHECK(int_vec != NULL);
    vec_push_back_i(int_vec, 10 * 10);
    vec_push_back_i(int_vec, -5);
    TEST_CHECK(vec_at_i(int_vec, 0) == 10 * 10);
    TEST_CHECK(vec_at_i(int_vec, 1) == -5);
    vec_free(int_vec);
}

void vecf(void) {
    TEST_CASE("enough capacity");
    Vector* float_vec = vec_create_type(5, VEC_FLOAT);
    TEST_CHECK(float_vec != NULL);
    vec_push_back_f(float_vec, 0.5332f);
    vec_push_back_f(float_vec, -1.5f);
    TEST_CHECK(vec_at_f(float_vec, 0) == 0.5332f);
    TEST_CHECK(vec_at_f(float_vec, 1) == -1.5f);
    vec_free(float_vec);

    TEST_CASE("no capacity");
    float_vec = vec_create_type(0, VEC_INT);
    TEST_CHECK(float_vec != NULL);
    vec_push_back_f(float_vec, 10.0);
    vec_push_back_f(float_vec, -5);
    TEST_CHECK(vec_at_f(float_vec, 0) == 10.0);
    TEST_CHECK(vec_at_f(float_vec, 1) == -5);

    vec_free(float_vec);
}

void test_vec_create_size(void) {
    TEST_CASE("capacity 0");
    Vector* v = vec_create_size(0, sizeof(long));
    TEST_CHECK(v != NULL);
    TEST_CHECK(v->capacity == 0);
    TEST_CHECK(v->data == NULL);
    TEST_CHECK(v->size == 0);
    TEST_CHECK(v->type_size == sizeof(long));
    vec_free(v);

    TEST_CASE("capacity 1");
    Vector* v2 = vec_create_size(1, sizeof(long));
    TEST_CHECK(v2 != NULL);
    TEST_CHECK(v2->capacity == 1);
    TEST_CHECK(v2->size == 0);
    TEST_CHECK(v2->type_size == sizeof(long));
    TEST_CHECK(v2->data != NULL);
    vec_free(v2);

    TEST_CASE("capacity 1, size 0");
    // we expect this to fail (return NULL), since objects of size 0 don't make sense
    Vector* v3 = vec_create_size(1, 0);
    TEST_CHECK(v3 == NULL);
    vec_free(v3);
}

void test_vec_create_type(void) {
    TEST_CASE("VEC_FLOAT, capacity 0");
    Vector* v = vec_create_type(0, VEC_FLOAT);
    TEST_CHECK(v != NULL);
    TEST_CHECK(v->type_size == sizeof(float));
    TEST_CHECK(v->capacity == 0);
    TEST_CHECK(v->size == 0);
    TEST_CHECK(v->data == NULL);

    TEST_CASE("vec_free sanity check");
    vec_free(v);
    TEST_CHECK(v == NULL);

    TEST_CASE("VEC_INT, capacity 0");
    v = vec_create_type(0, VEC_INT);
    TEST_CHECK(v != NULL);
    TEST_CHECK(v->type_size == sizeof(int));
    TEST_CHECK(v->capacity == 0);
    TEST_CHECK(v->size == 0);
    TEST_CHECK(v->data == NULL);
    vec_free(v);

    TEST_CASE("VEC_FLOAT, capacity >0");

    for (size_t i = 1; i < 5; ++i) {
        TEST_MSG("VEC_FLOAT, capacity %i", i);
        v = vec_create_type(i, VEC_FLOAT);
        TEST_CHECK(v != NULL);
        TEST_CHECK(v->type_size == sizeof(float));
        TEST_CHECK(v->capacity == i);
        TEST_CHECK(v->size == 0);
        TEST_CHECK(v->data != NULL);
        vec_free(v);
    }

    TEST_CASE("VEC_INT, capacity >0");

    for (size_t i = 1; i < 5; ++i) {
        TEST_MSG("VEC_INT, capacity %i", i);
        v = vec_create_type(i, VEC_INT);
        TEST_CHECK(v != NULL);
        TEST_CHECK(v->type_size == sizeof(int));
        TEST_CHECK(v->capacity == i);
        TEST_CHECK(v->size == 0);
        TEST_CHECK(v->data != NULL);
        vec_free(v);
    }
}

TEST_LIST = {
    { "create size", test_vec_create_size },
    { "create type", test_vec_create_type },
    { "int vector", veci },
    { "float vector", vecf },

    { NULL, NULL }
};
