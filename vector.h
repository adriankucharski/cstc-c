#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef vector_h
#define vector_h 1

void _cleanup_base(void *ptr);

// #define VECTOR_FUNCTION_PROTOTYPES(__TYPE__, __DECLARE_NAME__)                              \
//     int __optimize_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                         \
//     int __add_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                              \
//     void __cleanup##__DECLARE_NAME__(__DECLARE_NAME__ **vec);                               \
//     void __free_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                            \
//     int __empty##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                                   \
//     int __push##__DECLARE_NAME__(__DECLARE_NAME__ *vec, __TYPE__ element);                  \
//     int __insert##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index, __TYPE__ element);  \
//     int __pop##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                                     \
//     int __replace##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index, __TYPE__ element); \
//     void __clear##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                                  \
//     __TYPE__ __at##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index);                   \
//     __TYPE__ __front##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                              \
//     __TYPE__ __back##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                               \
//     void __foreach##__DECLARE_NAME__(__DECLARE_NAME__ *vec, void (*function)(__TYPE__));    \
//     __DECLARE_NAME__ *sized_##__DECLARE_NAME__(size_t initial_size);                        \
//     __DECLARE_NAME__ *new_##__DECLARE_NAME__();

// #define VECTOR_STRUCT_DECLARATION(__TYPE__, __DECLARE_NAME__)                   \
//     typedef struct __DECLARE_NAME__ __DECLARE_NAME__;                           \
//     struct __DECLARE_NAME__                                                     \
//     {                                                                           \
//         void (*free_memory)(__DECLARE_NAME__ * vec);                            \
//         size_t size;                                                            \
//         size_t __max_size;                                                      \
//         __TYPE__ *__data;                                                       \
//         int (*empty)(__DECLARE_NAME__ * vec);                                   \
//         int (*insert)(__DECLARE_NAME__ * vec, size_t index, __TYPE__ element);  \
//         int (*push)(__DECLARE_NAME__ * vec, __TYPE__ element);                  \
//         int (*pop)(__DECLARE_NAME__ * vec);                                     \
//         int (*replace)(__DECLARE_NAME__ * vec, size_t index, __TYPE__ element); \
//         __TYPE__ (*at)(__DECLARE_NAME__ * vec, size_t index);                   \
//         __TYPE__ (*front)(__DECLARE_NAME__ * vec);                              \
//         __TYPE__ (*back)(__DECLARE_NAME__ * vec);                               \
//         __TYPE__ (*element_constructor)(const __TYPE__ element);                \
//         void (*element_destructor)(__TYPE__ element);                           \
//         void (*clear)(__DECLARE_NAME__ * vec);                                  \
//         int (*add_memory)(__DECLARE_NAME__ * vec);                              \
//         int (*optimize_memory)(__DECLARE_NAME__ * vec);                         \
//         void (*foreach)(__DECLARE_NAME__ * vec, void (*function)(__TYPE__));    \
//         __DECLARE_NAME__ *(*clone)(const __DECLARE_NAME__ *vec);                \
//     };

/**
 * VECTOR macro should be called in global scope, not inside functions.
 *
 * Usage:
 * ```c
 *  VECTOR(int, vector_int, NULL, NULL); // vector of int is predefined
 *  vector_int *vec = new_vector_int();
 *  vec->push(vec, 75);                  // push 75 at end of vector   [75]
 *  vec->push(vec, 99);                  // push 99 at end of vector   [75, 99]
 *  vec->insert(vec, 1, 9);              // insert 9 at index 1        [75, 9, 99]
 *  int a = vec->back(vec);              // return 99 and assign it to variable a
 *  vec->free_memory(vec);               // free allocated memory
 * ```
 *
 *
 * @param __TYPE__                   Type of stored element e.g. int, double, char*, struct_name.
 * @param __DECLARE_NAME__           Name of vector declaration e.g. vector_int, vector_double.
 *                                   Used as: __DECLARE_NAME__ *vec = new___DECLARE_NAME__().
 * @param __ELEMENT_CONSTRUCTOR__    Copy constructor function for stored elements. If NULL, uses "=" operator,
 *                                   otherwise must be a function with signature: __TYPE__ constructor(const __TYPE__ element)
 *                                   For data types that use memory allocation, define a constructor that is called when push or insert.
 *                                   Example constructor for deep char* copy:
 *                                   char *charp_constructor(const char *element) {
 *                                        return strdup(element);
 *                                   }
 * @param __ELEMENT_DESTRUCTOR__     Destructor function. If NULL, does nothing,
 *                                   otherwise must be a function with signature: void destructor(__TYPE__ element)
 *                                   Called for each element when free_memory and clear are called or when an element is popped.
 *                                   Example destructor for char* vector:
 *                                   void charp_destructor(char *element) {
 *                                        free(element);
 *                                   }
 *
 * Example declaration:
 * ```c
 * VECTOR(char*, vector_charp, charp_constructor, charp_destructor);
 * ```
 * This creates a vector_charp type that stores deep-copied strings.
 *
 * The `scoped` macro can be used to automatically free the vector when it goes out of scope:
 * ```c
 * scoped vector_int *vec = new_vector_int();
 * ```
 *
 * @return This macro defines the functions and struct declarations for the specified vector type.
 */
#define VECTOR(__TYPE__, __DECLARE_NAME__, __ELEMENT_CONSTRUCTOR__, __ELEMENT_DESTRUCTOR__)  \
    typedef struct __DECLARE_NAME__ __DECLARE_NAME__;                                        \
    struct __DECLARE_NAME__                                                                  \
    {                                                                                        \
        void (*free_memory)(__DECLARE_NAME__ * vec);                                         \
        size_t size;                                                                         \
        size_t __max_size;                                                                   \
        __TYPE__ *__data;                                                                    \
        int (*empty)(__DECLARE_NAME__ * vec);                                                \
        int (*insert)(__DECLARE_NAME__ * vec, size_t index, __TYPE__ element);               \
        int (*push)(__DECLARE_NAME__ * vec, __TYPE__ element);                               \
        int (*pop)(__DECLARE_NAME__ * vec);                                                  \
        int (*replace)(__DECLARE_NAME__ * vec, size_t index, __TYPE__ element);              \
        __TYPE__ (*at)(__DECLARE_NAME__ * vec, size_t index);                                \
        __TYPE__ (*front)(__DECLARE_NAME__ * vec);                                           \
        __TYPE__ (*back)(__DECLARE_NAME__ * vec);                                            \
        __TYPE__ (*element_constructor)(const __TYPE__ element);                             \
        void (*element_destructor)(__TYPE__ element);                                        \
        void (*clear)(__DECLARE_NAME__ * vec);                                               \
        int (*add_memory)(__DECLARE_NAME__ * vec);                                           \
        int (*optimize_memory)(__DECLARE_NAME__ * vec);                                      \
        void (*foreach)(__DECLARE_NAME__ * vec, void (*function)(__TYPE__));                 \
        __DECLARE_NAME__ *(*clone)(const __DECLARE_NAME__ *vec);                             \
    };                                                                                       \
    int __optimize_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                          \
    int __add_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                               \
    void __cleanup##__DECLARE_NAME__(__DECLARE_NAME__ **vec);                                \
    void __free_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                             \
    int __empty##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                                    \
    int __push##__DECLARE_NAME__(__DECLARE_NAME__ *vec, __TYPE__ element);                   \
    int __insert##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index, __TYPE__ element);   \
    int __pop##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                                      \
    int __replace##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index, __TYPE__ element);  \
    void __clear##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                                   \
    __TYPE__ __at##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index);                    \
    __TYPE__ __front##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                               \
    __TYPE__ __back##__DECLARE_NAME__(__DECLARE_NAME__ *vec);                                \
    void __foreach##__DECLARE_NAME__(__DECLARE_NAME__ *vec, void (*function)(__TYPE__));     \
    __DECLARE_NAME__ *sized_##__DECLARE_NAME__(size_t initial_size);                         \
    __DECLARE_NAME__ *new_##__DECLARE_NAME__();                                              \
                                                                                             \
    int __add_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                \
    {                                                                                        \
        if (vec->size + 1 >= vec->__max_size)                                                \
        {                                                                                    \
            size_t new_max_size = vec->__max_size * 2 * sizeof(__TYPE__);                    \
            __TYPE__ *data = (__TYPE__ *)realloc(vec->__data, new_max_size);                 \
            if (data == NULL)                                                                \
                return 0;                                                                    \
            vec->__data = data;                                                              \
            vec->__max_size = vec->__max_size * 2;                                           \
        }                                                                                    \
        return 1;                                                                            \
    }                                                                                        \
    void __cleanup##__DECLARE_NAME__(__DECLARE_NAME__ **vec)                                 \
    {                                                                                        \
        if (vec && *vec)                                                                     \
        {                                                                                    \
            (*vec)->free_memory(*vec);                                                       \
            *vec = NULL;                                                                     \
        }                                                                                    \
    }                                                                                        \
    void __free_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                              \
    {                                                                                        \
        if (vec == NULL)                                                                     \
        {                                                                                    \
            return;                                                                          \
        }                                                                                    \
        if (vec->element_destructor)                                                         \
            for (size_t i = 0; i < vec->size; ++i)                                           \
                vec->element_destructor(vec->__data[i]);                                     \
        free(vec->__data);                                                                   \
        free(vec);                                                                           \
    }                                                                                        \
    int __empty##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                     \
    {                                                                                        \
        return vec->size == 0;                                                               \
    }                                                                                        \
    int __push##__DECLARE_NAME__(__DECLARE_NAME__ *vec, __TYPE__ element)                    \
    {                                                                                        \
        if (!vec->add_memory(vec))                                                           \
            return 0;                                                                        \
        if (vec->element_constructor)                                                        \
            vec->__data[vec->size++] = vec->element_constructor(element);                    \
        else                                                                                 \
            vec->__data[vec->size++] = element;                                              \
        return 1;                                                                            \
    }                                                                                        \
    int __insert##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index, __TYPE__ element)    \
    {                                                                                        \
        if (index > vec->size || vec->add_memory(vec) == 0)                                  \
            return 0;                                                                        \
        __TYPE__ *point = &vec->__data[index];                                               \
        memmove(point + 1, point, (vec->size - index) * sizeof(__TYPE__));                   \
        ++vec->size;                                                                         \
        if (vec->element_constructor)                                                        \
            vec->__data[index] = vec->element_constructor(element);                          \
        else                                                                                 \
            vec->__data[index] = element;                                                    \
        return 1;                                                                            \
    }                                                                                        \
    int __pop##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                       \
    {                                                                                        \
        if (vec->size == 0)                                                                  \
            return 0;                                                                        \
        --vec->size;                                                                         \
        if (vec->element_destructor)                                                         \
            vec->element_destructor(vec->__data[vec->size]);                                 \
        return 1;                                                                            \
    }                                                                                        \
    int __replace##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index, __TYPE__ element)   \
    {                                                                                        \
        if (index >= vec->size)                                                              \
            return 0;                                                                        \
        if (element == vec->__data[index])                                                   \
            return 1;                                                                        \
                                                                                             \
        if (vec->element_destructor)                                                         \
            vec->element_destructor(vec->__data[index]);                                     \
                                                                                             \
        if (vec->element_constructor)                                                        \
            vec->__data[index] = vec->element_constructor(element);                          \
        else                                                                                 \
            vec->__data[index] = element;                                                    \
                                                                                             \
        return 1;                                                                            \
    }                                                                                        \
    void __clear##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                    \
    {                                                                                        \
        if (vec->element_destructor)                                                         \
            for (size_t i = 0; i < vec->size; ++i)                                           \
                vec->element_destructor(vec->__data[i]);                                     \
        vec->size = 0;                                                                       \
    }                                                                                        \
    __TYPE__ __at##__DECLARE_NAME__(__DECLARE_NAME__ *vec, size_t index)                     \
    {                                                                                        \
        assert(index < vec->size);                                                           \
        return vec->__data[index];                                                           \
    }                                                                                        \
    __TYPE__ __front##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                \
    {                                                                                        \
        assert(vec->size > 0);                                                               \
        return vec->__data[0];                                                               \
    }                                                                                        \
    __TYPE__ __back##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                                 \
    {                                                                                        \
        assert(vec->size > 0);                                                               \
        return vec->__data[vec->size - 1];                                                   \
    }                                                                                        \
    void __foreach##__DECLARE_NAME__(__DECLARE_NAME__ *vec, void (*function)(__TYPE__))      \
    {                                                                                        \
        for (size_t i = 0; i < vec->size; ++i)                                               \
            function(vec->__data[i]);                                                        \
    }                                                                                        \
    __DECLARE_NAME__ *__clone##__DECLARE_NAME__(const __DECLARE_NAME__ *vec)                 \
    {                                                                                        \
        if (vec == NULL)                                                                     \
            return NULL;                                                                     \
        __DECLARE_NAME__ *new_vec = sized_##__DECLARE_NAME__(vec->size > 0 ? vec->size : 2); \
        if (new_vec == NULL)                                                                 \
            return NULL;                                                                     \
        for (size_t i = 0; i < vec->size; ++i)                                               \
        {                                                                                    \
            if (vec->element_constructor)                                                    \
                new_vec->__data[i] = vec->element_constructor(vec->__data[i]);               \
            else                                                                             \
                new_vec->__data[i] = vec->__data[i];                                         \
        }                                                                                    \
        new_vec->size = vec->size;                                                           \
        return new_vec;                                                                      \
    }                                                                                        \
    int __optimize_memory##__DECLARE_NAME__(__DECLARE_NAME__ *vec)                           \
    {                                                                                        \
        if (vec->size == 0)                                                                  \
            return 1;                                                                        \
        __TYPE__ *data = realloc(vec->__data, sizeof(__TYPE__) * vec->size);                 \
        if (data == NULL)                                                                    \
            return 0;                                                                        \
        vec->__data = data;                                                                  \
        vec->__max_size = vec->size;                                                         \
        return 1;                                                                            \
    }                                                                                        \
    __DECLARE_NAME__ *sized_##__DECLARE_NAME__(size_t initial_size)                          \
    {                                                                                        \
        __DECLARE_NAME__ *vec = (__DECLARE_NAME__ *)calloc(1, sizeof(__DECLARE_NAME__));     \
        vec->empty = __empty##__DECLARE_NAME__;                                              \
        vec->push = __push##__DECLARE_NAME__;                                                \
        vec->insert = __insert##__DECLARE_NAME__;                                            \
        vec->pop = __pop##__DECLARE_NAME__;                                                  \
        vec->replace = __replace##__DECLARE_NAME__;                                          \
        vec->clear = __clear##__DECLARE_NAME__;                                              \
        vec->at = __at##__DECLARE_NAME__;                                                    \
        vec->front = __front##__DECLARE_NAME__;                                              \
        vec->back = __back##__DECLARE_NAME__;                                                \
        vec->free_memory = __free_memory##__DECLARE_NAME__;                                  \
        vec->add_memory = __add_memory##__DECLARE_NAME__;                                    \
        vec->element_constructor = __ELEMENT_CONSTRUCTOR__;                                  \
        vec->element_destructor = __ELEMENT_DESTRUCTOR__;                                    \
        vec->foreach = __foreach##__DECLARE_NAME__;                                          \
        vec->clone = __clone##__DECLARE_NAME__;                                              \
        vec->size = 0;                                                                       \
        vec->optimize_memory = __optimize_memory##__DECLARE_NAME__;                          \
        vec->__max_size = initial_size;                                                      \
        vec->__data = (__TYPE__ *)malloc(sizeof(__TYPE__) * initial_size);                   \
        return (vec->__data == NULL) ? NULL : vec;                                           \
    }                                                                                        \
    __DECLARE_NAME__ *new_##__DECLARE_NAME__()                                               \
    {                                                                                        \
        return sized_##__DECLARE_NAME__(2);                                                  \
    }

/**
 * Predefined for automatic cleanup
 *
 * Usage:
 * ```c
 * {
 *   scoped vector_int *vec = new_vector_int(); // vec will be automatically freed when it goes out of scope
 *   vec->push(vec, 75);                         // push 75 at end of vector   [75]
 *   vec->push(vec, 99);                         // push 99 at end of vector   [75, 99]
 *   vec->insert(vec, 1, 9);                     // insert 9 at index 1        [75, 9, 99]
 *   int a = vec->back(vec);                     // return 99 and assign it to variable a
 *   // Memory will be automatically freed here
 * }
 * ```
 *
 * @return This macro defines a scoped variable that will be automatically cleaned up when it goes out of scope.
 */
#define scoped __attribute__((cleanup(_cleanup_base)))

/**
 * A trick to make the vector type generic.
 * As long as the `_base_vector` and `__DECLARE_NAME__` structs have the first member
 * `(*free_memory)` - the compiler will treat them as the same type.
 * This allows us to use the same cleanup function for all vector types.
 */
void _cleanup_base(void *ptr)
{
    struct _base_vector
    {
        void (*free_memory)(void *vec);
    } **_ptr = ptr;
    if (_ptr && *_ptr)
    {
        (*_ptr)->free_memory(*_ptr);
        *_ptr = NULL;
    }
}

/**
 * Predefined vector types for primitives.
 */

VECTOR(int, vector_int, NULL, NULL);
VECTOR(char, vector_char, NULL, NULL);
VECTOR(float, vector_float, NULL, NULL);
VECTOR(double, vector_double, NULL, NULL);

/**
 * Predefined vector types for char*.
 * The char* vector uses strdup for deep copy and free for cleanup.
 * This is useful for storing strings in the vector.
 * The charp_constructor and charp_destructor functions are defined here for convenience.
 *
 * If scoped(vector_charp) is used, the vector will be automatically freed when it goes out of scope.
 * This is useful for managing memory automatically and avoiding memory leaks.
 */
void charp_destructor(char *element) { free(element); }
char *charp_constructor(const char *element)
{
    char *copy = calloc(strlen(element) + 1, sizeof(char));
    if (copy)
        strncpy(copy, element, strlen(element) + 1);
    return copy;
}
VECTOR(char *, vector_charp, charp_constructor, charp_destructor);

#endif
