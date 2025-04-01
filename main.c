#include <stdio.h>
#include "vector.h"

void vector_int_destructor(vector_int *vec) { vec->free_memory(vec); }
vector_int *vector_int_constructor(const vector_int *vec) { return vec->clone(vec); }

VECTOR(vector_int *, vv_int, vector_int_constructor, vector_int_destructor);

void show_vector_int(vector_int *vec)
{
    printf("Vector size: %zu\n", vec->size);
    for (size_t i = 0; i < vec->size; ++i)
        printf("%d ", vec->at(vec, i));
    printf("\n");
}

int main()
{
    {
        printf("Beginning of the program\n");
        scoped vector_int *vec1 = new_vector_int();
        vector_int *vec2 = new_vector_int();
        scoped vector_int *vec3 = new_vector_int();
        vec1->push(vec1, 1);
        vec1->push(vec1, 1);
        vec1->push(vec1, 3);
        vec2->push(vec2, 2);
        vec2->push(vec2, 2);
        vec2->push(vec2, 3);
        vec3->push(vec3, 3);

        scoped vv_int *vec = new_vv_int();

        for (size_t i = 0; i < 10; ++i)
        {
            vec->push(vec, vec1);
            vec->push(vec, vec2);
        }

        vec2->free_memory(vec2);

        vec1->replace(vec1, 0, 10);
        vec->replace(vec, 0, vec3);
        vec->foreach (vec, show_vector_int);

        printf("End of the program\n");
    }

    return 0;
}