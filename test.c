#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "vector.h"

int rand_int(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

/* INT VECTOR */
void TEST1()
{
    printf("TEST: %s\n", __func__);
    vector_int *vec = new_vector_int();
    int array[] = {124, 125, 643, 12, 1425, 51, 34, 562, 12, 432, 523};

    for (int i = 0; i < sizeof(array) / sizeof(int); ++i)
        vec->push(vec, array[i]);

    for (int i = 0; i < sizeof(array) / sizeof(int); ++i)
        assert(vec->at(vec, i) == array[i]);

    vec->free_memory(vec);
}

void TEST2()
{
    printf("TEST: %s\n", __func__);
    vector_int *vec = new_vector_int();
    int array[150] = {0};

    for (int i = 0; i < 150; ++i)
    {
        int k = rand() % __INT32_MAX__;

        array[i] = k;
        vec->push(vec, k);
    }

    for (int i = 0; i < 150; ++i)
        assert(vec->at(vec, i) == array[i]);

    vec->free_memory(vec);
}

void TEST3()
{
    printf("TEST: %s\n", __func__);
    vector_int *vec = new_vector_int();

    int start = rand();
    int end = rand();

    vec->insert(vec, 0, start);
    int limit = rand_int(50, 500);
    for (int i = 0; i < limit; ++i)
    {
        int k = rand_int(0, __INT32_MAX__);
        int index = rand_int(1, vec->size);

        vec->insert(vec, index, k);
    }
    vec->insert(vec, vec->size, end);

    assert(vec->at(vec, 0) == start);
    assert(vec->front(vec) == start);

    assert(vec->at(vec, vec->size - 1) == end);
    assert(vec->back(vec) == end);

    vec->free_memory(vec);
}

void TEST4()
{
    printf("TEST: %s\n", __func__);
    vector_int *vec = new_vector_int();

    assert(vec->pop(vec) == 0);
    assert(vec->empty(vec) == 1);

    int limit = rand_int(50, 500);
    for (int i = 0; i < limit; ++i)
        vec->push(vec, i);

    while (vec->pop(vec) == 1)
        ;

    vec->free_memory(vec);
}

void TEST5()
{
    printf("TEST: %s\n", __func__);
    vector_int *vec = new_vector_int();

    int limit = rand_int(50, 500);
    for (int i = 0; i < limit; ++i)
        vec->push(vec, i);

    vec->clear(vec);
    assert(vec->pop(vec) == 0);
    assert(vec->empty(vec) == 1);

    vec->free_memory(vec);
}

//*CHAR* VECTOR*//
void TEST6()
{
    printf("TEST: %s\n", __func__);
    vector_charp *vec = new_vector_charp();

    char str[] = "//*CHAR* VECTOR*//";
    vec->push(vec, str);

    assert(vec->empty(vec) == 0);

    assert(strcmp(vec->at(vec, 0), str) == 0);
    assert(strcmp(vec->front(vec), str) == 0);
    assert(strcmp(vec->back(vec), str) == 0);

    str[0] = 'p';
    assert(strcmp(vec->at(vec, 0), str) != 0);
    assert(strcmp(vec->front(vec), str) != 0);
    assert(strcmp(vec->back(vec), str) != 0);

    assert(vec->pop(vec) == 1);
    assert(vec->empty(vec) == 1);

    vec->free_memory(vec);
}

void TEST7()
{
    printf("TEST: %s\n", __func__);
    vector_charp *vec = new_vector_charp();

    int len_words = rand_int(300, 600);
    char **words = calloc(len_words, sizeof(char *));

    for (int i = 0; i < len_words; ++i)
    {
        int len_word = rand_int(2, 15);
        char *word = calloc(len_word + 1, sizeof(char));

        for (int k = 0; k < len_word; ++k)
            word[k] = rand_int('A', 'Z');

        words[i] = word;
        vec->push(vec, word);
    }

    for (int i = 0; i < len_words; ++i)
        assert(strcmp(vec->at(vec, i), words[i]) == 0);

    for (int i = 0; i < len_words; ++i)
    {
        words[i][0] = words[i][0] + 1;
    }

    for (int i = 0; i < len_words; ++i)
        assert(strcmp(vec->at(vec, i), words[i]) != 0);

    for (int i = 0; i < len_words; ++i)
        free(words[i]);
    free(words);
    vec->free_memory(vec);
}

int main()
{
    srand(time(NULL));
    //*INT VECTOR*//
    TEST1();
    TEST2();
    TEST3();
    TEST4();
    TEST5();

    TEST6();
    TEST7();

    printf("All tests have been completed sucesfull\n");
    return 0;
}
