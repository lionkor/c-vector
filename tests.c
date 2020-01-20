#include "acutest/include/acutest.h"
#include <time.h>
#include <stdlib.h>
#include "Vector.h"

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

void test_mass_push_back(void) {
    TEST_CASE("capacity 0");
    clock_t start = clock();
    Vector* v     = vec_create_type(0, VEC_INT);
    for (int i = 0; i < 2000; ++i) {
        vec_push_back_i(v, i);
        TEST_CHECK(vec_at_i(v, v->size - 1) == i);
    }
    clock_t end = clock();
    vec_free(v);

    // printf("\nfirst run took %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    TEST_CASE("capacity >0");
    start = clock();
    v     = vec_create_type(2001, VEC_INT);
    for (int i = 0; i < 2000; ++i) {
        vec_push_back_i(v, i);
        TEST_CHECK(vec_at_i(v, v->size - 1) == i);
    }
    end = clock();
    vec_free(v);

    // printf("\nsecond run took %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
}

TEST_LIST = {
    { "create size", test_vec_create_size },
    { "create type", test_vec_create_type },
    { "int vector", veci },
    { "float vector", vecf },
    { "mass push back", test_mass_push_back },

    { NULL, NULL }
};
