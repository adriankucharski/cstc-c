#include <stdlib.h>
#include <string.h>
#ifndef vector_h
#define vector_h 1

/**
* VECTOR macro should be call in global scope, not inside functions.
* 
* Usage: 
* VECTOR(int, vector_int, NULL, NULL); \\vector of int is predefined
* vector_int *vec = new_vector_int();
* vec->push(vec,75);         //push 75 at end of vector   [75]
* vec->push(vec,99);         //push 99 at end of vector   [75, 99]
* vec->insert(vec,1,9);      //insert 9 at index 1        [75, 9, 99]
* int a = vec->back(vec);    //return 99 and assign it to variable a
* vec->free_memory(vec);     //free alocated memory 
*
* @param __TYPE__                   type of stored element e.g. [int, double, char*].
* @param __DECLARE_NAME__           name of vector declaration e.g. __DECLARE_NAME__ == vector_int_t -> [vector_int_t *vec = new vector_int_t()].
* @param __ELEMENT_CONSTRUCTOR__    copy constructor function for stored elements. If NULL use "=" operator, otherwise have to be [__TYPE__ constructor(const __TYPE__ element)] function
* For data type that uses memory allocation we can define construtor that is call when push or insert.
* e.g. constroctor for deep char* copy
*       char *charp_constructor(char *element){
*            char *s = (char*)malloc((strlen(element) + 1) * sizeof(char));
*            return strcpy(s, element);
*       }
* @param __ELEMENT_DESTRUCTOR__     destructor function. If NULL do nothing, otherwise have to be [void destructor(__TYPE__ element)] function.
* Call for each element when free_memory and clear are called or poped element whlile pop.
* e.g. destroctor free char* vector
*       char *charp_destructor(char *element){
*            free(element);
*       }
* @param VECTOR(char*,vector_charp_t,charp_constructor,charp_destructor); 
* Call macro this way, now you can use "vector_charp_t*" that stores deep copied char*.
* @return Macro writes functions and struct declarations for given type of arguments. 
* For int vector it may look -> VECTOR(int, vector_int_t, NULL, NULL) ->  vector_int_t *vec = new vector_int_t().
*/
#define VECTOR(__TYPE__, __DECLARE_NAME__, __ELEMENT_CONSTRUCTOR__, __ELEMENT_DESTRUCTOR__) \
    typedef struct __DECLARE_NAME__ __DECLARE_NAME__;                                       \
    struct __DECLARE_NAME__                                                                 \
    {                                                                                       \
        size_t size;                                                                        \
        size_t __max_size;                                                                  \
        __TYPE__ *__data;                                                                   \
        int (*empty)(__DECLARE_NAME__ * vec);                                               \
        int (*insert)(__DECLARE_NAME__ * vec, size_t index, __TYPE__ element);              \
        int (*push)(__DECLARE_NAME__ * vec, __TYPE__ element);                              \
        int (*pop)(__DECLARE_NAME__ * vec);                                                 \
        __TYPE__(*at)                                                                       \
        (__DECLARE_NAME__ * vec, size_t index);                                             \
        __TYPE__(*front)                                                                    \
        (__DECLARE_NAME__ * vec);                                                           \
        __TYPE__(*back)                                                                     \
        (__DECLARE_NAME__ * vec);                                                           \
        __TYPE__(*element_constructor)                                                      \
        (const __TYPE__ element);                                                           \
        void (*element_destructor)(__TYPE__ element);                                       \
        void (*free_memory)(__DECLARE_NAME__ * vec);                                        \
        void (*clear)(__DECLARE_NAME__ * vec);                                              \
        int (*add_memory)(__DECLARE_NAME__ * vec);                                          \
        void (*foreach)(__DECLARE_NAME__ * vec, void (*function)(__TYPE__));                \
    };                                                                                      \
    int __add_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                               \
    {                                                                                       \
        if (vec->size + 1 >= vec->__max_size)                                               \
        {                                                                                   \
            size_t new_max_size = vec->__max_size * 2 * sizeof(__TYPE__);                   \
            __TYPE__ *data = (__TYPE__ *)realloc(vec->__data, new_max_size);                \
            if (data == NULL)                                                               \
                return 0;                                                                   \
            vec->__data = data;                                                             \
            vec->__max_size = vec->__max_size * 2;                                          \
        }                                                                                   \
        return 1;                                                                           \
    }                                                                                       \
    void __free_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                             \
    {                                                                                       \
        if (vec->element_destructor)                                                        \
            for (size_t i = 0; i < vec->size; ++i)                                          \
                vec->element_destructor(vec->__data[i]);                                    \
        free(vec->__data);                                                                  \
        free(vec);                                                                          \
    }                                                                                       \
    int __empty##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                    \
    {                                                                                       \
        return vec->size == 0;                                                              \
    }                                                                                       \
    int __push##__DECLARE_NAME__(__DECLARE_NAME__ *vec, __TYPE__ element)                   \
    {                                                                                       \
        vec->add_memory(vec);                                                               \
        if (vec->element_constructor)                                                       \
            vec->__data[vec->size++] = vec->element_constructor(element);                   \
        else                                                                                \
            vec->__data[vec->size++] = element;                                             \
        return 1;                                                                           \
    }                                                                                       \
    int __insert##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index, __TYPE__ element)   \
    {                                                                                       \
        if (index > vec->size || vec->add_memory(vec) == 0)                                 \
            return 0;                                                                       \
        __TYPE__ *point = &vec->__data[index];                                              \
        memmove(point + 1, point, (vec->size - index) * sizeof(__TYPE__));                  \
        ++vec->size;                                                                        \
        if (vec->element_constructor)                                                       \
            vec->__data[index] = vec->element_constructor(element);                         \
        else                                                                                \
            vec->__data[index] = element;                                                   \
        return 1;                                                                           \
    }                                                                                       \
    int __pop##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                      \
    {                                                                                       \
        if (vec->size == 0)                                                                 \
            return 0;                                                                       \
        --vec->size;                                                                        \
        if (vec->element_destructor)                                                        \
            vec->element_destructor(vec->__data[vec->size]);                                \
        return 1;                                                                           \
    }                                                                                       \
    void __clear##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                   \
    {                                                                                       \
        if (vec->element_destructor)                                                        \
            for (size_t i = 0; i < vec->size; ++i)                                          \
                vec->element_destructor(vec->__data[i]);                                    \
        vec->size = 0;                                                                      \
    }                                                                                       \
    __TYPE__ __at##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index)                    \
    {                                                                                       \
        return vec->__data[index];                                                          \
    }                                                                                       \
    __TYPE__ __front##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                               \
    {                                                                                       \
        return vec->__data[0];                                                              \
    }                                                                                       \
    __TYPE__ __back##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                \
    {                                                                                       \
        return vec->__data[vec->size - 1];                                                  \
    }                                                                                       \
    void __foreach##__DECLARE_NAME__(__DECLARE_NAME__ *vec, void (*function)(__TYPE__))     \
    {                                                                                       \
        for (size_t i = 0; i < vec->size; ++i)                                              \
            function(vec->__data[i]);                                                       \
    }                                                                                       \
    __DECLARE_NAME__ *new_##__DECLARE_NAME__()                                              \
    {                                                                                       \
        __DECLARE_NAME__ *vec = (__DECLARE_NAME__ *)calloc(1, sizeof(__DECLARE_NAME__));    \
        vec->empty = __empty##__DECLARE_NAME__;                                             \
        vec->push = __push##__DECLARE_NAME__;                                               \
        vec->insert = __insert##__DECLARE_NAME__;                                           \
        vec->pop = __pop##__DECLARE_NAME__;                                                 \
        vec->clear = __clear##__DECLARE_NAME__;                                             \
        vec->at = __at##__DECLARE_NAME__;                                                   \
        vec->front = __front##__DECLARE_NAME__;                                             \
        vec->back = __back##__DECLARE_NAME__;                                               \
        vec->free_memory = __free_memory##__DECLARE_NAME__;                                 \
        vec->add_memory = __add_memory##__DECLARE_NAME__;                                   \
        vec->element_constructor = __ELEMENT_CONSTRUCTOR__;                                 \
        vec->element_destructor = __ELEMENT_DESTRUCTOR__;                                   \
        vec->foreach = __foreach##__DECLARE_NAME__;                                         \
        vec->size = 0;                                                                      \
        vec->__max_size = 2;                                                                \
        vec->__data = (__TYPE__ *)malloc(sizeof(__TYPE__) * vec->__max_size);               \
        return (vec->__data == NULL) ? NULL : vec;                                          \
    }

/*##############################*/
/*Predefined for basic datatypes*/
/*##############################*/
VECTOR(int, vector_int, NULL, NULL);
VECTOR(char, vector_char, NULL, NULL);
VECTOR(float, vector_float, NULL, NULL);
VECTOR(double, vector_double, NULL, NULL);


/*#######################################*/
/*Predefined for deep copy char* datatype*/
/*#######################################*/
char *charp_constructor(const char *element)
{
    return strdup(element);
}
void charp_destructor(char *element)
{
    free(element);
}
VECTOR(char *, vector_charp, charp_constructor, charp_destructor);

#endif
